# 6Harmonics Inc
# maintainer: Qige Zhao
# updated on: 2017.03.15

APP=app-demo

all: $(APP)

$(APP): src/*.c #src/include/*.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -I. -o $@ $^ -L.

clean:
	rm -f $(APP) src/*.o

