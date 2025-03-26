
#include "lexbor/selectors/selectors.h"
#include "lexbor/css/css.h"
#include "lexbor/html/html.h"
#include <stdexcept>
#include <string>

class Selectors {
  private:
    lxb_selectors_t *selectors = nullptr;

  public:
    Selectors() {
        selectors = lxb_selectors_create();
        if (lxb_selectors_init(selectors) != LXB_STATUS_OK) {
            throw std::runtime_error("Failed to init Selectors");
        }
    }

    ~Selectors() {
        if (selectors) {
            lxb_selectors_destroy(selectors, true);
        }
    }

    template <typename F>
    void find_nodes(lxb_html_document_t *doc, lxb_css_selector_list_t *list,
                    F &&find_callback) {
        struct Ctx {
            F func;
        };

        auto thunk = [](lxb_dom_node_t *node, lxb_css_selector_specificity_t,
                        void *ctx_void) -> lxb_status_t {
            auto *ctx = static_cast<Ctx *>(ctx_void);
            return ctx->func(node) ? LXB_STATUS_OK : LXB_STATUS_STOP;
        };

        Ctx ctx{std::forward<F>(find_callback)};
        lxb_selectors_find(selectors, lxb_dom_interface_node(doc), list, thunk,
                           &ctx);
    }

    lxb_selectors_t *raw() const { return selectors; }
};