CREATE TRIGGER queue_notify_event AFTER INSERT ON dm.dm_queue FOR EACH ROW EXECUTE PROCEDURE dm.queue_event();
