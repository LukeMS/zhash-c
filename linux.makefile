zhash = libzhash.a

$(zhash): $(obj)
	$(AR) rcs $@ $^

.PHONY: install
install: $(zhash)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include
	cp $(zhash) $(DESTDIR)$(PREFIX)/lib/$(zhash)
	$(info cp $(zhash) $(DESTDIR)$(PREFIX)/lib/$(zhash))
	cp inc/zhash.h $(DESTDIR)$(PREFIX)/include/
	$(info cp inc/zhash.h $(DESTDIR)$(PREFIX)/include/)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/$(zhash)
	rm -f $(DESTDIR)$(PREFIX)/include/zhash.h