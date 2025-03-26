
#include <stdexcept>
#include <string>
#include "lexbor/html/html.h"
#include "lexbor/css/css.h"
#include "lexbor/selectors/selectors.h"

class CSSParser
{
private:
    lxb_css_parser_t *parser = nullptr;

public:
    CSSParser()
    {
        parser = lxb_css_parser_create();
        if (lxb_css_parser_init(parser, NULL) != LXB_STATUS_OK)
        {
            throw std::runtime_error("Failed to init HTML parser");
        }
    }

    ~CSSParser()
    {
        if (parser)
        {
            lxb_css_parser_destroy(parser, true);
        }
    }

    lxb_css_selector_list_t *selectors_parse(const std::string &selectors) const
    {
        lxb_css_selector_list_t *list = lxb_css_selectors_parse(parser, reinterpret_cast<const lxb_char_t *>(selectors.c_str()),
                                                                selectors.size());
        if (parser->status != LXB_STATUS_OK)
        {
            throw std::runtime_error("CSS Selectors parse failed");
        }

        return list;
    }

    lxb_css_parser_t *raw() const { return parser; }
};