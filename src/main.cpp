#include "CLI11.hpp"
#include <iostream>

#include "config.cpp"
#include "file.cpp"
#include "formatter.hpp"
#include "lib/lexbor/CSSParser.cpp"
#include "lib/lexbor/HTMLParser.cpp"
#include "lib/lexbor/Selectors.cpp"

Config config;

inline lxb_status_t callback(const lxb_char_t *data, size_t len, void *ctx) {
    printf("%.*s", (int)len, (const char *)data);

    return LXB_STATUS_OK;
}

int main(int argc, char **argv) {
    // Parse arguments
    CLI::App app{"HTML Query"};

    argv = app.ensure_utf8(argv);

    app.add_flag("--text", config.text, "Output text");
    app.add_flag("--inner", config.inner, "Inner HTML");
    app.add_flag("--first", config.first, "Only show first result");
    app.add_option("-f,--file", config.filename, "File to read");
    app.add_option("-a,--attr", config.attr, "Attribute");
    app.add_option("selector", config.slctrs, "Selector");
    app.add_option("--limit", config.limit, "Limit the number of results");
    app.add_option("--base_url", config.base_url, "Base url for links");
    app.add_flag("--json", config.json, "Output in JSON format");

    CLI11_PARSE(app, argc, argv);

    int count = 0;
    try {
        // Check for empty missing selector
        if (config.slctrs.empty()) {
            throw std::runtime_error("Missing selector");
        }

        // Read file contents
        std::string file_contents = read_input(config.filename);

        HTMLParser html_parser;
        CSSParser css_parser;
        Selectors selectors;

        /* Parse and get the log. */
        lxb_css_selector_list_t *list =
            css_parser.selectors_parse(config.slctrs);

        /* Parse html to document */
        lxb_html_document_t *doc = html_parser.parse(file_contents);

        Formatter::OutputType output_type = Formatter::OutputType::OUTER_HTML;
        if (config.text) {
            output_type = Formatter::OutputType::TEXT;
        } else if (config.inner) {
            output_type = Formatter::OutputType::INNER_HTML;
        } else if (!config.attr.empty()) {
            output_type = Formatter::OutputType::ATTRIBUTE;
        } else if (config.json) {
            output_type = Formatter::OutputType::JSON;
        }

        Formatter formatter(output_type, config.attr, config.base_url);
        formatter.setLimit(config.limit);
        formatter.setJson(config.json);

        selectors.find_nodes(doc, list, [&](lxb_dom_node_t *node) {
            formatter.processNode(node);
            return config.first == false;
        });

        formatter.printJson();
    } catch (std::runtime_error err) {
        std::cerr << "Error: " << err.what() << "\n";
        exit(2);
    }

    return count ? 0 : 1;
}
