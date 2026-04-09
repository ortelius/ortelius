// Package kafka provides Kafka event processing functionality for the PDVD backend.
package kafka

import (
	"context"

	"crypto/tls" // Add this
	"log"
	"os"
	"strings"
	"time"

	"github.com/ortelius/pdvd-backend/v12/database"
	release "github.com/ortelius/pdvd-backend/v12/events/modules/releases"
	"github.com/ortelius/pdvd-backend/v12/internal/services"
	"github.com/segmentio/kafka-go"
	"github.com/segmentio/kafka-go/sasl/plain" // Add this
)

// RunEventProcessor starts a Kafka event processor that listens for release events
// and processes them using the provided database connection.
func RunEventProcessor(ctx context.Context, db database.DBConnection) error {
	brokersEnv := os.Getenv("KAFKA_BROKERS")

	var brokers []string
	if brokersEnv != "" {
		brokers = strings.Split(brokersEnv, ",")
	} else {
		brokers = []string{"localhost:9092"}
	}

	// 1. Configure SASL/PLAIN using Environment Variables
	username := os.Getenv("KAFKA_API_KEY")
	password := os.Getenv("KAFKA_API_SECRET")

	var dialer *kafka.Dialer

	// Only configure SASL/TLS if credentials are provided
	if username != "" && password != "" {
		mechanism := plain.Mechanism{
			Username: username,
			Password: password,
		}

		dialer = &kafka.Dialer{
			Timeout:       10 * time.Second,
			DualStack:     true,
			SASLMechanism: mechanism,
			TLS:           &tls.Config{}, // Confluent Cloud requires TLS
		}
	} else {
		// Default dialer for local development (no SASL/TLS)
		dialer = &kafka.Dialer{
			Timeout:   10 * time.Second,
			DualStack: true,
		}
	}

	topic := "release-events"
	var conn *kafka.Conn
	var err error

	// Retry logic: 3 tries
	for i := 1; i <= 3; i++ {
		log.Printf("Kafka connection attempt %d/3...", i)
		// Use the configured dialer (with SASL/TLS) for the check
		conn, err = dialer.DialContext(ctx, "tcp", brokers[0])
		if err == nil {
			conn.Close()
			break
		}
		if i < 3 {
			time.Sleep(2 * time.Second)
		}
	}

	if err != nil {
		return err
	}

	// 2. Configure the Reader to use the Dialer
	readerConfig := kafka.ReaderConfig{
		Brokers:  brokers,
		GroupID:  "pdvd-backend-worker",
		Topic:    topic,
		MaxBytes: 10e6,
		Dialer:   dialer, // Inject the secure dialer here
	}

	reader := kafka.NewReader(readerConfig)

	go func() {
		defer reader.Close()
		service := &services.ReleaseServiceWrapper{DB: db}
		fetcher := &services.CIDFetcher{}

		log.Println("Kafka Event Processor started. Listening for release events...")

		for {
			select {
			case <-ctx.Done():
				return
			default:
				msg, err := reader.ReadMessage(ctx)
				if err != nil {
					if ctx.Err() != nil {
						return
					}
					continue
				}
				_ = release.HandleReleaseSBOMCreatedWithService(ctx, msg.Value, fetcher, service)
			}
		}
	}()

	return nil
}
