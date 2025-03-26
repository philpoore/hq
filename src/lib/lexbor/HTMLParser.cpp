
#include <stdexcept>
#include <string>
#include "lexbor/html/html.h"
#include "lexbor/css/css.h"
#include "lexbor/selectors/selectors.h"

class HTMLParser
{
private:
    lxb_html_parser_t *parser = nullptr;

public:
    HTMLParser()
    {
        parser = lxb_html_parser_create();
        if (lxb_html_parser_init(parser) != LXB_STATUS_OK)
        {
            throw std::runtime_error("Failed to init HTML parser");
        }
    }

    ~HTMLParser()
    {
        if (parser)
        {
            lxb_html_parser_destroy(parser);
        }
    }

    lxb_html_document_t *parse(const std::string &html) const
    {
        lxb_html_document_t *doc = lxb_html_parse(
            parser,
            reinterpret_cast<const lxb_char_t *>(html.c_str()),
            html.size());

        if (!doc)
            throw std::runtime_error("HTML parse failed");

        return doc;
    }

    lxb_html_parser_t *raw() const { return parser; }
};