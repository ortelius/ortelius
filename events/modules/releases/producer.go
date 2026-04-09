// Package release handles Kafka event production for release SBOM creation events.
package release

import (
	"context"
	"crypto/tls"
	"encoding/json"
	"os"
	"time"

	"github.com/google/uuid"
	"github.com/ortelius/pdvd-backend/v12/model"
	"github.com/segmentio/kafka-go"
	"github.com/segmentio/kafka-go/sasl/plain"
)

// ReleaseProducer handles sending SBOM creation events to Kafka
//
//revive:disable-next-line:exported
type ReleaseProducer struct {
	Writer *kafka.Writer
}

// NewReleaseProducer initializes a new Kafka writer for release events
func NewReleaseProducer(brokers []string, topic string) *ReleaseProducer {

	username := os.Getenv("KAFKA_API_KEY")
	password := os.Getenv("KAFKA_API_SECRET")

	var transport *kafka.Transport

	if username != "" && password != "" {
		mechanism := plain.Mechanism{
			Username: username,
			Password: password,
		}

		transport = &kafka.Transport{
			SASL: mechanism,
			TLS:  &tls.Config{},
		}
	} else {
		transport = &kafka.Transport{}
	}

	return &ReleaseProducer{
		Writer: &kafka.Writer{
			Addr:      kafka.TCP(brokers...),
			Topic:     topic,
			Balancer:  &kafka.LeastBytes{},
			Transport: transport, // Add the transport here
		},
	}
}

// PublishReleaseSBOMCreated sends the event to the Kafka topic
func (p *ReleaseProducer) PublishReleaseSBOMCreated(ctx context.Context, release model.ProjectRelease, cid string) error {

	// Construct the Event Contract
	event := ReleaseSBOMCreatedEvent{
		EventType:     "release.sbom.created",
		EventID:       uuid.New().String(),
		EventTime:     time.Now().UTC(),
		SchemaVersion: "v1",
		Release:       release,
		SBOMRef: SBOMReference{
			CID:         cid,
			StorageType: "ipfs", // Default storage type for the system
			UploadedAt:  time.Now().UTC(),
		},
	}

	// Marshal to JSON
	payload, err := json.Marshal(event)
	if err != nil {
		return err
	}

	// Write to Kafka
	return p.Writer.WriteMessages(ctx, kafka.Message{
		Key:   []byte(release.Name),
		Value: payload,
	})
}

// Close cleans up the Kafka writer
func (p *ReleaseProducer) Close() error {
	return p.Writer.Close()
}
