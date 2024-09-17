ALTER TABLE dm.dm_componentitem
ADD COLUMN IF NOT EXISTS ScoreCardPinned boolean,
ADD COLUMN IF NOT EXISTS Score float,
ADD COLUMN IF NOT EXISTS Maintained float,
ADD COLUMN IF NOT EXISTS CodeReview float,
ADD COLUMN IF NOT EXISTS CIIBestPractices float,
ADD COLUMN IF NOT EXISTS License float,
ADD COLUMN IF NOT EXISTS SignedReleases float,
ADD COLUMN IF NOT EXISTS DangerousWorkflow float,
ADD COLUMN IF NOT EXISTS Packaging float,
ADD COLUMN IF NOT EXISTS TokenPermissions float,
ADD COLUMN IF NOT EXISTS BranchProtection float,
ADD COLUMN IF NOT EXISTS BinaryArtifacts float,
ADD COLUMN IF NOT EXISTS PinnedDependencies float,
ADD COLUMN IF NOT EXISTS SecurityPolicy float,
ADD COLUMN IF NOT EXISTS Fuzzing float,
ADD COLUMN IF NOT EXISTS SAST float,
ADD COLUMN IF NOT EXISTS Vulnerabilities float;
