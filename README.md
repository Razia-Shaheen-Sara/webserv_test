Phase 1 — Hardcoded router (no config)

Hardcode root = ./www
Map / → index.html, everything else → ./www + path
Test with curl: 200 on existing file, 404 on missing

Phase 2 — Config parsing (isolated)

Read the config file
Fill ServerConfig and LocationConfig structs
Test by printing what you parsed — no routing yet

Phase 3 — Connect config to router

Router reads root from config instead of hardcoded string
Longest-prefix location matching
Test same curl cases but now driven by config

Phase 4 — One advanced feature at a time

Redirect (simplest — just return a code + new URL)
Autoindex (directory listing)
Upload/POST handling
CGI (most complex — save for last)
