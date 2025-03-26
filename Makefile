BUILD_DIR=build
DEPS_BUILD_DIR=.build-deps

all: libs build compile

libs: lib/CLI11.hpp lib/nlohmann_json.hpp

lib/CLI11.hpp:
	./scripts/fetch_lib_CLI11.sh
lib/nlohmann_json.hpp:
	./scripts/fetch_lib_nlohmann_json.sh

build:
	mkdir -p $(BUILD_DIR)

compile: build
	cd $(BUILD_DIR) ; \
	cmake -DFETCHCONTENT_BASE_DIR=../$(DEPS_BUILD_DIR) .. ; \
	cmake --build .

run:
	./$(BUILD_DIR)/hq

clean:
	rm -rf $(BUILD_DIR)

clean-deps:
	rm -rf $(DEPS_BUILD_DIR)

.PHONY: compile clean clean-deps libs