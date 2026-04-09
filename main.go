// Package main is the entry point for the PDVD backend application.
package main

import (
	"context"
	"log"
	"os"
	"os/signal"
	"syscall"

	"github.com/ortelius/pdvd-backend/v12/database"
	"github.com/ortelius/pdvd-backend/v12/internal/api"
	"github.com/ortelius/pdvd-backend/v12/internal/kafka"
)

func main() {
	db := database.InitializeDatabase()

	ctx, cancel := signal.NotifyContext(context.Background(), os.Interrupt, syscall.SIGTERM)
	defer cancel()

	// 1. Initialize Fiber App FIRST
	app := api.NewFiberApp(db)
	port := os.Getenv("MS_PORT")
	if port == "" {
		port = "3000"
	}

	// 2. Start Server in Goroutine
	// This ensures the API is listening immediately
	go func() {
		log.Printf("Starting server on port %s", port)
		if err := app.Listen(":" + port); err != nil {
			log.Fatalf("Failed to start server: %v", err)
		}
	}()

	// 3. Attempt to start Kafka (Optional) - Moved AFTER Fiber
	// We run this in a separate goroutine so it doesn't block the main thread
	// or delay the application if Kafka is unavailable.
	go func() {
		if err := kafka.RunEventProcessor(ctx, db); err != nil {
			log.Printf("Warning: Kafka initialization failed after 3 tries: %v. Starting without Kafka support.", err)
		} else {
			log.Println("Kafka processor initialized successfully.")
		}
	}()

	<-ctx.Done()
	log.Println("Shutting down pdvd-backend...")
	app.Shutdown()
}
