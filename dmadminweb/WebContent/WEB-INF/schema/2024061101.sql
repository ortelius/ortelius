ALTER TABLE dm.dm_componentitem
ADD COLUMN IF NOT EXISTS CITests float,
ADD COLUMN IF NOT EXISTS Contributors float,
ADD COLUMN IF NOT EXISTS DependencyUpdateTool float,
ADD COLUMN IF NOT EXISTS SBOM float,
ADD COLUMN IF NOT EXISTS Webhooks float;
