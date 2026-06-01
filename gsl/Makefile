CC = gcc


all: dotproduct mean gradient interp_linear interp_spline interp_poly fft

dotproduct: dotproduct.c
	$(CC) $< -Wall -O2 -ffast-math -DHAVE_INLINE -lgsl -o $@

mean: mean.c
	$(CC) $< -Wall -O2 -ffast-math -DHAVE_INLINE -lgsl -o $@

gradient: gradient.c
	$(CC) $< -Wall -O2 -ffast-math -DHAVE_INLINE -lgsl -o $@

interp_linear: interp_linear.c
	$(CC) $< -Wall -O2 -ffast-math -DHAVE_INLINE -lgsl -lm -o $@

interp_spline: interp_spline.c
	$(CC) $< -Wall -O2 -ffast-math -DHAVE_INLINE -lgsl -lm -o $@

interp_poly: interp_poly.c
	$(CC) $< -Wall -O2 -ffast-math -DHAVE_INLINE -lgsl -lm -o $@

fft: fft.c
	$(CC) $< -Wall -O2 -ffast-math -DHAVE_INLINE -lgsl -lm -o $@

clean:
	rm -f dotproduct mean gradient interp_linear interp_spline interp_poly fft
