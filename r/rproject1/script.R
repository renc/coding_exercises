mu <- 50
stddev <- 1
N <- 10000
pop <- rnorm(N, mean = mu, sd = stddev)

n <- 30
samp.means <- rnorm(N, mean = mu, sd = stddev / sqrt(n))
max.samp.means <- max(density(samp.means)$y)


plot(density(pop), main = "Population Density",
    ylim = c(0, max.samp.means),
     xlab = "X", ylab = "")
lines(density(samp.means))
