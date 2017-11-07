zhash = libzhash.a

$(zhash): $(obj)
	$(AR) rcs $@ $^

.PHONY: install
install: $(zhash)
	mkdir -p /usr/lib
	mkdir -p /usr/include
	cp $(zhash) /usr/lib/$(zhash)
	cp inc/zhash.h /usr/include/

.PHONY: uninstall
uninstall:
	rm -f /usr/lib/$(zhash)
	rm -f /usr/include/zhash.h