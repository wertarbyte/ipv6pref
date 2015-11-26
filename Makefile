LIBRARY=ipv6pref.so

$(LIBRARY): ipv6pref.c
	$(CC) $< -shared -ldl -o $@

clean:
	-rm $(LIBRARY)
