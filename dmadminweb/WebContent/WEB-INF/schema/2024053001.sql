update dm.dm_componentitem set purl = 'pkg:docker/' || dockerrepo || '@' || dockertag where purl is null and dockerrepo is not null and dockertag is not null;
