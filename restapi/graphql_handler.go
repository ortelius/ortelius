// Package restapi provides HTTP handlers for the REST API including GraphQL support.
package restapi

import (
	"context"

	"github.com/gofiber/fiber/v3"
	"github.com/graphql-go/graphql"
	"github.com/ortelius/pdvd-backend/v12/restapi/modules/auth"
)

// GraphQLHandler returns a Fiber handler for GraphQL requests
func GraphQLHandler(schema graphql.Schema) fiber.Handler {
	return func(c fiber.Ctx) error {
		var params struct {
			Query         string                 `json:"query"`
			OperationName string                 `json:"operationName"`
			Variables     map[string]interface{} `json:"variables"`
		}

		if err := c.Bind().Body(&params); err != nil {
			return c.Status(fiber.StatusBadRequest).JSON(fiber.Map{
				"errors": []map[string]interface{}{{"message": "Invalid request body"}},
			})
		}

		opName := params.OperationName
		if opName == "" {
			opName = "-"
		}
		c.Locals("graphql_op", opName)

		ctx := context.Background()
		if username, ok := c.Locals("username").(string); ok {
			ctx = context.WithValue(ctx, auth.UserKey, username)
		}
		if role, ok := c.Locals("role").(string); ok {
			ctx = context.WithValue(ctx, auth.RoleKey, role)
		}
		if orgs, ok := c.Locals("orgs").([]string); ok {
			ctx = context.WithValue(ctx, auth.OrgsKey, orgs)
		}

		result := graphql.Do(graphql.Params{
			Schema:         schema,
			RequestString:  params.Query,
			VariableValues: params.Variables,
			OperationName:  params.OperationName,
			Context:        ctx,
		})

		return c.JSON(result)
	}
}
