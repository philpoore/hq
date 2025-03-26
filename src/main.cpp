#include <iostream>
#include "CLI11.hpp"

#include "file.cpp"
#include "lexbor/html/html.h"
#include "lexbor/css/css.h"
#include "lexbor/selectors/selectors.h"

lxb_inline lxb_status_t
serializer_callback(const lxb_char_t *data, size_t len, void *ctx)
{
    printf("%.*s", (int)len, (const char *)data);

    return LXB_STATUS_OK;
}

lxb_status_t
callback(const lxb_char_t *data, size_t len, void *ctx)
{
    printf("%.*s", (int)len, (const char *)data);

    return LXB_STATUS_OK;
}

lxb_status_t
find_callback(lxb_dom_node_t *node, lxb_css_selector_specificity_t spec,
              void *ctx)
{
    unsigned *count = reinterpret_cast<unsigned int *>(ctx);

    (*count)++;

    // printf("%u) ", *count);
    (void)lxb_html_serialize_cb(node, callback, NULL);
    printf("\n");

    return LXB_STATUS_OK;
}

int main(int argc, char **argv)
{
    CLI::App app{"HTML Query"};
    argv = app.ensure_utf8(argv);

    std::string filename = "-";
    std::string slctrs = "";
    app.add_option("-f,--file", filename, "File to read");
    app.add_option("selector", slctrs, "Selector");

    CLI11_PARSE(app, argc, argv);

    std::string file_contents = read_input(filename);

    // std::cout << "file: " << filename << "\n";
    // std::cout << "selector: " << slctrs << "\n";
    // std::cout << "file_contents length: " << file_contents.size() << "\n";
    // std::cout << "file_contents:\n"
    //           << file_contents << "\n";

    // Create HTML parser
    lxb_html_parser_t *html_parser = lxb_html_parser_create();
    {
        lxb_status_t status = lxb_html_parser_init(html_parser);
        if (status != LXB_STATUS_OK)
        {
            std::cerr << "Failed to create HTML parser\n";
            exit(1);
        }
    }

    // Create CSS parser
    lxb_css_parser_t *css_parser = lxb_css_parser_create();
    {
        lxb_status_t status = lxb_css_parser_init(css_parser, NULL);
        if (status != LXB_STATUS_OK)
        {
            std::cerr << "Failed to create CSS parser\n";
            exit(1);
        }
    }

    // Create CSS selectors
    lxb_selectors_t *selectors = lxb_selectors_create();
    {
        lxb_status_t status = lxb_selectors_init(selectors);
        if (status != LXB_STATUS_OK)
        {
            std::cerr << "Failed to create selectors\n";
            exit(1);
        }
    }

    /* Parse and get the log. */
    lxb_css_selector_list_t *list = lxb_css_selectors_parse(css_parser, reinterpret_cast<const lxb_char_t *>(slctrs.c_str()),
                                                            slctrs.size());
    if (css_parser->status != LXB_STATUS_OK)
    {
        return EXIT_FAILURE;
    }

    /* Selector List Serialization. */
    // printf("Selectors: ");
    // (void)lxb_css_selector_serialize_list_chain(list, callback, NULL);
    // printf("\n");

    /* Parse html to document */
    lxb_status_t status;
    lxb_html_document_t *doc = lxb_html_parse(html_parser, reinterpret_cast<const lxb_char_t *>(file_contents.c_str()), file_contents.size());
    if (doc == NULL)
    {
        std::cerr << "Failed to create Document object";
        exit(1);
    }

    /* Find HTML nodes by CSS Selectors. */

    // printf("Found:\n");
    int count = 0;

    status = lxb_selectors_find(selectors, lxb_dom_interface_node(doc), list, find_callback, &count);
    if (status != LXB_STATUS_OK)
    {
        return EXIT_FAILURE;
    }

    // status = lxb_html_serialize_pretty_tree_cb(lxb_dom_interface_node(doc),
    //                                            LXB_HTML_SERIALIZE_OPT_UNDEF,
    //                                            0, serializer_callback, NULL);

    return 0;
}
