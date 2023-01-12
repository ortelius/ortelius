DROP TRIGGER if exists deployment_insert_trigger on dm.dm_deployment;

CREATE TRIGGER deployment_insert_trigger
  AFTER INSERT
  ON dm.dm_deployment
  FOR EACH ROW
  EXECUTE PROCEDURE snapshot_comps_for_deployment();
