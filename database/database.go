// Package database - Handles all interaction with ArangoDB
package database

import (
	"context"
	"crypto/tls"
	"fmt"
	"net"
	"net/http"
	"os"
	"time"

	"github.com/arangodb/go-driver/v2/arangodb"
	"github.com/arangodb/go-driver/v2/connection"
	"github.com/cenkalti/backoff"
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
)

var logger = InitLogger() // setup the logger

// DBConnection is the structure that defined the database engine and collections
type DBConnection struct {
	Collections map[string]arangodb.Collection
	Database    arangodb.Database
}

// indexConfig defines a struct to hold the index definition
type indexConfig struct {
	Collection string
	IdxName    string
	IdxField   []string // Supported multiple fields for composite indexes
	Unique     bool
}

var initDone = false          // has the data been initialized
var dbConnection DBConnection // database connection definition

// GetEnvDefault is a convenience function for handling env vars
func GetEnvDefault(key, defVal string) string {
	val, ex := os.LookupEnv(key) // get the env var
	if !ex {                     // not found return default
		return defVal
	}
	return val // return value for env var
}

// InitLogger sets up the Zap Logger to log to the console in a human readable format
func InitLogger() *zap.Logger {
	prodConfig := zap.NewProductionConfig()
	prodConfig.Encoding = "console"
	prodConfig.EncoderConfig.EncodeTime = zapcore.ISO8601TimeEncoder
	prodConfig.EncoderConfig.EncodeDuration = zapcore.StringDurationEncoder
	logger, _ := prodConfig.Build()
	return logger
}

func dbConnectionConfig(endpoint connection.Endpoint, dbuser string, dbpass string) connection.HttpConfiguration {
	return connection.HttpConfiguration{
		Authentication: connection.NewBasicAuth(dbuser, dbpass),
		Endpoint:       endpoint,
		ContentType:    connection.ApplicationJSON,
		Transport: &http.Transport{
			TLSClientConfig: &tls.Config{
				InsecureSkipVerify: true, // #nosec G402
			},
			DialContext: (&net.Dialer{
				Timeout:   30 * time.Second,
				KeepAlive: 90 * time.Second,
			}).DialContext,
			MaxIdleConns:          100,
			IdleConnTimeout:       90 * time.Second,
			TLSHandshakeTimeout:   10 * time.Second,
			ExpectContinueTimeout: 1 * time.Second,
		},
	}
}

// InitializeDatabase is the function for connecting to the db engine, creating the database and collections
func InitializeDatabase() DBConnection {
	const initialInterval = 10 * time.Second
	const maxInterval = 2 * time.Minute

	var db arangodb.Database
	var collections map[string]arangodb.Collection
	const databaseName = "vulnmgt"

	ctx := context.Background()

	if initDone {
		return dbConnection
	}

	False := false
	True := true
	dbhost := GetEnvDefault("ARANGO_HOST", "localhost")
	dbport := GetEnvDefault("ARANGO_PORT", "8529")
	dbuser := GetEnvDefault("ARANGO_USER", "root")
	dbpass := GetEnvDefault("ARANGO_PASS", "mypassword")
	dburl := GetEnvDefault("ARANGO_URL", "http://"+dbhost+":"+dbport)

	var client arangodb.Client

	//
	// Database connection with backoff retry
	//

	bo := backoff.NewExponentialBackOff()
	bo.InitialInterval = initialInterval
	bo.MaxInterval = maxInterval
	bo.MaxElapsedTime = 0 // Set to 0 for indefinite retries

	err := backoff.RetryNotify(func() error {
		fmt.Println("Attempting to connect to ArangoDB")
		endpoint := connection.NewRoundRobinEndpoints([]string{dburl})
		conn := connection.NewHttpConnection(dbConnectionConfig(endpoint, dbuser, dbpass))

		client = arangodb.NewClient(conn)

		// Ask the version of the server
		versionInfo, err := client.Version(ctx)
		if err != nil {
			return err
		}

		logger.Sugar().Infof("Database has version '%s' and license '%s'\n", versionInfo.Version, versionInfo.License)
		return nil

	}, bo, func(err error, _ time.Duration) {
		fmt.Printf("Retrying connection to ArangoDB: %v\n", err)
	})

	if err != nil {
		logger.Sugar().Fatalf("Backoff Error %v\n", err)
	}

	//
	// Database creation
	//

	exists := false
	dblist, _ := client.Databases(ctx)

	for _, dbinfo := range dblist {
		if dbinfo.Name() == databaseName {
			exists = true
			break
		}
	}

	if exists {
		var options arangodb.GetDatabaseOptions
		if db, err = client.GetDatabase(ctx, databaseName, &options); err != nil {
			logger.Sugar().Fatalf("Failed to get Database: %v", err)
		}
	} else {
		if db, err = client.CreateDatabase(ctx, databaseName, nil); err != nil {
			logger.Sugar().Fatalf("Failed to create Database: %v", err)
		}
	}

	//
	// Collection creation for document storage
	//

	collections = make(map[string]arangodb.Collection)
	// Added "users", "invitations", "roles", and "orgs" for user management
	collectionNames := []string{"release", "sbom", "purl", "cve", "endpoint", "sync", "metadata", "cve_lifecycle", "users", "invitations", "roles", "orgs"}

	for _, collectionName := range collectionNames {
		var col arangodb.Collection

		exists, _ = db.CollectionExists(ctx, collectionName)
		if exists {
			var options arangodb.GetCollectionOptions
			if col, err = db.GetCollection(ctx, collectionName, &options); err != nil {
				logger.Sugar().Fatalf("Failed to use collection: %v", err)
			}
		} else {
			if col, err = db.CreateCollectionV2(ctx, collectionName, nil); err != nil {
				logger.Sugar().Fatalf("Failed to create collection: %v", err)
			}
		}

		collections[collectionName] = col
	}

	//
	// Edge collection creation
	//

	edgeCollectionNames := []string{"release2sbom", "sbom2purl", "cve2purl", "release2cve"}

	for _, edgeCollectionName := range edgeCollectionNames {
		var col arangodb.Collection

		exists, _ = db.CollectionExists(ctx, edgeCollectionName)
		if exists {
			var options arangodb.GetCollectionOptions
			if col, err = db.GetCollection(ctx, edgeCollectionName, &options); err != nil {
				logger.Sugar().Fatalf("Failed to use edge collection: %v", err)
			}
		} else {
			edgeType := arangodb.CollectionTypeEdge
			if col, err = db.CreateCollectionV2(ctx, edgeCollectionName, &arangodb.CreateCollectionPropertiesV2{
				Type: &edgeType,
			}); err != nil {
				logger.Sugar().Fatalf("Failed to create edge collection: %v", err)
			}
		}

		collections[edgeCollectionName] = col
	}

	//
	// Index creation for document collections
	//

	idxList := []indexConfig{
		// Original CVE indexes
		{Collection: "cve", IdxName: "package_name", IdxField: []string{"affected[*].package.name"}, Unique: false},
		{Collection: "cve", IdxName: "package_purl", IdxField: []string{"affected[*].package.purl"}, Unique: false},
		{Collection: "cve", IdxName: "cve_id", IdxField: []string{"id"}, Unique: false},

		// SBOM & Release indexes
		{Collection: "sbom", IdxName: "sbom_contentsha", IdxField: []string{"contentsha"}, Unique: false},
		{Collection: "release", IdxName: "release_contentsha", IdxField: []string{"contentsha"}, Unique: false},

		// Composite index for latest version retrieval per org + name
		{Collection: "release", IdxName: "release_org_name_version_order",
			IdxField: []string{
				"org",
				"name",
				"version_major",
				"version_minor",
				"version_patch",
				"version_prerelease",
				"version",
			}, Unique: false},

		// PURL unique index
		{Collection: "purl", IdxName: "purl_idx", IdxField: []string{"purl"}, Unique: true},

		// User Management Indexes
		{Collection: "users", IdxName: "users_username", IdxField: []string{"username"}, Unique: true},
		{Collection: "users", IdxName: "users_email", IdxField: []string{"email"}, Unique: true},
		{Collection: "invitations", IdxName: "idx_token", IdxField: []string{"token"}, Unique: true},
		{Collection: "invitations", IdxName: "idx_invitation_username", IdxField: []string{"username"}, Unique: false},
		{Collection: "invitations", IdxName: "idx_expires_at", IdxField: []string{"expires_at"}, Unique: false},

		// CVE Lifecycle indexes
		{Collection: "cve_lifecycle", IdxName: "lifecycle_cve_id", IdxField: []string{"cve_id"}, Unique: false},
		{Collection: "cve_lifecycle", IdxName: "lifecycle_remediated", IdxField: []string{"is_remediated"}, Unique: false},
		{Collection: "cve_lifecycle", IdxName: "lifecycle_endpoint_release_version",
			IdxField: []string{"endpoint_name", "release_name", "introduced_version"}, Unique: false},

		// Sync indexes
		// Composite index including endpoint_name for optimized queries
		{Collection: "sync", IdxName: "sync_endpoint_release",
			IdxField: []string{"endpoint_name", "release_name"}, Unique: false},
		// Exact index for release_name + release_version (matches old db.sync.ensureIndex)
		{Collection: "sync", IdxName: "sync_release_name_version",
			IdxField: []string{"release_name", "release_version"}, Unique: false},
	}

	for _, idx := range idxList {
		found := false

		if indexes, err := collections[idx.Collection].Indexes(ctx); err == nil {
			for _, index := range indexes {
				if idx.IdxName == index.Name {
					found = true
					break
				}
			}
		}

		if !found {
			unique := False
			if idx.Unique {
				unique = True
			}
			indexOptions := arangodb.CreatePersistentIndexOptions{
				Unique: &unique,
				Sparse: &False,
				Name:   idx.IdxName,
			}
			_, _, err = collections[idx.Collection].EnsurePersistentIndex(ctx, idx.IdxField, &indexOptions)
			if err != nil {
				logger.Sugar().Fatalln("Error creating index:", err)
			} else {
				logger.Sugar().Infof("Created index: %s on %s.%v", idx.IdxName, idx.Collection, idx.IdxField)
			}
		}
	}

	initDone = true

	dbConnection = DBConnection{
		Database:    db,
		Collections: collections,
	}

	logger.Sugar().Infof("Database initialization complete with user management support")

	return dbConnection
}

// FindReleaseByCompositeKey retrieves a release key based on unique identifiers
func FindReleaseByCompositeKey(ctx context.Context, db arangodb.Database, name, version, contentSha string) (string, error) {
	query := `
		FOR r IN release
			FILTER r.name == @name 
			   AND r.version == @version 
			   AND r.contentsha == @contentsha
			LIMIT 1
			RETURN r._key
	`
	bindVars := map[string]interface{}{
		"name":       name,
		"version":    version,
		"contentsha": contentSha,
	}

	cursor, err := db.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: bindVars,
	})
	if err != nil {
		return "", err
	}
	defer cursor.Close()

	if cursor.HasMore() {
		var key string
		_, err := cursor.ReadDocument(ctx, &key)
		if err != nil {
			return "", err
		}
		return key, nil
	}

	return "", nil
}

// FindSBOMByContentHash retrieves an SBOM document from the database by its content hash
func FindSBOMByContentHash(ctx context.Context, db arangodb.Database, contentHash string) (string, error) {
	query := `
		FOR s IN sbom
			FILTER s.contentsha == @hash
			LIMIT 1
			RETURN s._key
	`
	bindVars := map[string]interface{}{
		"hash": contentHash,
	}

	cursor, err := db.Query(ctx, query, &arangodb.QueryOptions{
		BindVars: bindVars,
	})
	if err != nil {
		return "", err
	}
	defer cursor.Close()

	if cursor.HasMore() {
		var key string
		_, err := cursor.ReadDocument(ctx, &key)
		if err != nil {
			return "", err
		}
		return key, nil
	}

	return "", nil
}
