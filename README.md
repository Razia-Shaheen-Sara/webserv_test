##Learning steps:
Step 1 (must)
HTTP basics (very short)
Search:
“HTTP request response basics”
MDN Web Docs (best)
Step 2 (important)
nginx location matching behavior
Search:
“nginx location priority explained”
Step 3 (practice)
run nginx locally
test with curl / browser / telnet
Step 4 (optional RFC)

Only read:

RFC 9110 (HTTP semantics)

But only for reference, not study

##---TO DO--

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


##Test MY ROUTER

🧪 1. Multiple locations (core idea: longest match)
LocationConfig loc1;
loc1.path = "/";
loc1.root = "./www";

LocationConfig loc2;
loc2.path = "/images";
loc2.root = "./www/images";

config.locations.push_back(loc1);
config.locations.push_back(loc2);

req.path = "/images/cat.jpg";

👉 Expect:

Root: ./www/images
FullPath: ./www/images/cat.jpg
🧪 2. Test longest match more deeply
LocationConfig loc3;
loc3.path = "/images/cats";
loc3.root = "./www/cats";

config.locations.push_back(loc3);

req.path = "/images/cats/kitty.jpg";

👉 Should pick /images/cats, NOT /images

🧪 3. No matching location
config.locations.clear();

LocationConfig loc;
loc.path = "/images";
loc.root = "./www/images";

config.locations.push_back(loc);

req.path = "/videos/movie.mp4";

👉 Expect:

No match → defaults used
Root: ./www
🧪 4. Test / special case
req.path = "/";

👉 Expect:

FullPath: ./www/index.html
🧪 5. Remove index in location
loc.index = "";
req.path = "/";

👉 See what happens
👉 You should notice fallback behavior

🧪 6. Test autoindex
loc.autoindex = true;
req.path = "/images/";

👉 Print:

std::cout << "Autoindex: " << d.autoindex << std::endl;
🧪 7. Test redirect
loc.redirectCode = 301;
loc.redirectUrl = "http://example.com";

👉 Print:

std::cout << "Redirect: " << d.redirectCode << " -> " << d.redirectUrl << std::endl;
🧪 8. Test methods
loc.methods.push_back("GET");
loc.methods.push_back("POST");

👉 Print:

for (size_t i = 0; i < d.methods.size(); i++)
    std::cout << d.methods[i] << " ";
🧪 9. Edge case (important bug)
loc.path = "/images";
req.path = "/images123";

👉 See:

It will match ❌ (this is a bug)
🧪 10. Empty root in location
loc.root = "";

👉 Should fallback to:

./www
🧠 What you should observe

Each test teaches one thing:

Test	Learn
1	basic matching
2	longest match
3	fallback to default
4	/ handling
5	index fallback
6	flags (autoindex)
7	redirect logic
8	methods
9	matching bug
10	root fallback
🔥 Best way to learn

After each run, ask yourself:

👉 “Why did it choose THIS location?”

If you can answer that → Router is clear.


Here’s a **tricky test that exposes a real router bug** (this breaks many beginner implementations).

---

## 🧪 Trick test: partial prefix bug

### Setup

```cpp
LocationConfig loc1;
loc1.path = "/images";
loc1.root = "./www/images";

LocationConfig loc2;
loc2.path = "/img";
loc2.root = "./www/img";

config.locations.push_back(loc1);
config.locations.push_back(loc2);
```

### Request

```cpp
req.path = "/imageset/cat.jpg";
```

---

## ❗ What your router will do (current code)

Because you use:

```cpp
uri.find(loc.path) == 0
```

👉 `/imageset` starts with `/images` ❌ MATCHES WRONG

So result becomes:

```text
Root: ./www/images   ❌ WRONG
```

---

## 🧠 Why this is wrong

You matched:

```
/images   → matches /imageset (bad)
```

But correct behavior should be:

👉 only match full folder segment

---

## ✅ Correct rule (what real servers do)

Match only if:

```text
/images/  OR exact /images
```

---

## 🔧 Fix (simple version)

Replace:

```cpp
if (uri.find(loc.path) == 0)
```

with:

```cpp
if (uri.compare(0, loc.path.size(), loc.path) == 0 &&
    (uri.size() == loc.path.size() || uri[loc.path.size()] == '/'))
```

---

## 🧪 Re-test same case

Now:

```cpp
/imageset/cat.jpg
```

👉 should NOT match `/images`

---

## 🔥 Why this test is important

It teaches you:

👉 prefix match is NOT enough in routing
👉 you need **boundary awareness**

---

## 🧠 Real-world analogy

Wrong:

```
"car" matches "carpet"
```

Correct:

```
"car" only matches "car/..."
```

---

