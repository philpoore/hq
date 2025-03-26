# hq (HTML Query)

**hq** is a lightweight command-line tool that parses HTML and extracts elements using CSS selectors. Inspired by tools like `jq` for JSON, **hq** makes it easy to extract content, attributes, and structure from HTML directly in your terminal.

It follows the Unix philosophy — use `curl` or other tools to fetch HTML, then pipe it into `hq` for querying.

---

## 📦 Features

- Accepts input from `stdin` or a file
- Supports standard CSS selectors (`div`, `a[href]`, `.class`, etc.)
- Extracts:
  - Full HTML nodes (outer HTML)
  - Inner HTML
  - Text content
  - Specific attributes (e.g., `href`, `src`, `target`)
- Return all matches (default) or just the first
- Designed to be simple, fast, and composable with Unix tools

---

## 🖥️ Usage Examples

```bash
# Extract the page title
curl http://example.com | hq "title"
# => <title>Example Domain</title>

# Get all image sources from a page
curl http://example.com | hq "img" --attr src
# => /images/logo.png
# => /images/banner.jpg

# Get just the first <h1>
cat page.html | hq "h1" --first
# => <h1>Welcome!</h1>

# Read from file instead of stdin
hq --file index.html "a.button" --attr href
# => /signup
# => /login
```

## 🛠 Installation
Note: This tool is under development. Build instructions will be added soon.
Once released:

```bash
git clone https://github.com/philpoore/hq.git
cd hq
make
```

## ⚙️ CLI Options

Option	Description
```
--file FILE	Read HTML from a file instead of stdin
--attr ATTR	Return only the value of a specific attribute
--inner	Return only the innerHTML of matched nodes
--text	Return only text content of matched nodes
--first	Return only the first match
--pretty	Pretty-print the output (optional)
```
## 🧪 Development Status
hq is still in early development. Contributions, ideas, and feedback are welcome!

## ✅ TODO: MVP Tasks
- [x] Accept HTML from stdin or file (--file)
- [x] Parse HTML using an external parser (e.g., Gumbo)
- [x] Support basic CSS selectors
- [x] Return full outer HTML of all matching elements
- [x] Add support for:
    - [x] `--inner` for innerHTML
    - [x] `--attr` for specific attributes
    - [x] `--text` for inner text
    - [x] `--first` to only return the first match
- [x] Build simple CLI interface (manual parsing or use CLI11)
- [x] Handle invalid selectors or bad HTML gracefully

## ✨ Nice To Have (Future Ideas)
- [x] `--json` output (useful for piping to other tools)
- [ ] Fetch URL directly via `--url` (using libcurl)
- [ ] Regex filtering on the output
- [x] Option to resolve relative URLs with a `--base-url`
- [ ] Pretty print / syntax-highlighted output
- [ ] Interactive mode (`hq -i`)
- [ ] Selector profiling or benchmarking
- [ ] Output format templates (e.g., custom formatting)

## 🧠 Philosophy
Keep it small. Keep it fast. Do one thing well.

## 📄 License
MIT