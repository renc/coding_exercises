
#Íê³É•rég
x <- c("1.23.10", "1.23.21", "1.28.32", "1.36.35")
x <- matrix(x, nc=1)

# x is time interval which finish matrix
# x is in one race
time.2.pro <- function(x, shrunk = 4000) {
	
	len <- nrow(x)
	
	x.last <- strsplit(x[len,],"\\.")
	last.time <- as.numeric(x.last[[1]][1]) * 60 * 1000 + as.numeric(x.last[[1]][2]) * 1000 + as.numeric(x.last[[1]][3])
		
	tot.interval <- 0
	pro <- interval <- matrix(NA, nr= len, nc=1)
	for (i in 1:len) {
		x.sl <- strsplit(x[i,],"\\.")
		interval[i,1] <- 2 * last.time  - (as.numeric(x.sl[[1]][1]) * 60 * 1000 + as.numeric(x.sl[[1]][2]) * 1000 + as.numeric(x.sl[[1]][3]) )
	#cat(interval[i,1], "\n")
		
		interval[i,1] <- 3 ^ (interval[i,1] / shrunk) #^ mi
		
		tot.interval <- tot.interval + interval[i,1] 
	}
	for (i in 1:len) {
		pro[i,1] <- interval[i,1] / tot.interval
	}	
	return(pro)
}


# test example
#   x.t <- matrix(c(100000,90234,89872,100005, 77653),nc=1)

interval.2.pro <- function(x, shrunk = 1000) {
	
	len <- nrow(x)
	x[is.na(x)] <- 0
	max.interval <- max(x, na.rm=T)
	
	tot.interval <- 0
	pro <- interval <- matrix(NA, nr= len, nc=1)
	
	for (i in 1:len) {
		interval[i,1] <- max.interval - x[i,1]
		
		#interval[i,1] <- 2 ^ (interval[i,1] / shrunk) 
		
		tot.interval <- tot.interval + interval[i,1] 
	}
	for (i in 1:len) {
		pro[i,1] <- interval[i,1] / tot.interval
	}	
	return(pro)
	
}
#p <- interval.2.pro(x.t, 1000)
#p


# x is matrix which time interval of finish
# x is in one race
time.2.interval <- function(x) {
	
	len <- nrow(x)
	
	x.last <- strsplit(x[len,],"\\.")
	last.time <- as.numeric(x.last[[1]][1]) * 60 * 1000 + as.numeric(x.last[[1]][2]) * 1000 + as.numeric(x.last[[1]][3])
		
	interval <- matrix(NA, nr= len, nc=1)
	for (i in 1:len) {
		x.sl <- strsplit(x[i,],"\\.")
		interval[i,1] <- 2 * last.time  - (as.numeric(x.sl[[1]][1]) * 60 * 1000 + as.numeric(x.sl[[1]][2]) * 1000 + as.numeric(x.sl[[1]][3]) )
	}
	return(interval)
}

p <- time.2.pro(x, shrunk = 5000)
cat(p)


# demean load / weight
load.weight <- function(load, weight) {

	len <- nrow(load)
	x <- load / weight
	
	x.mean <- mean(x, na.rm=T)
	
	x.demean <- x - x.mean
	colnames(x.demean) <- "load.weight"
	return(x.demean)
}

x1 <- c("a", "b", "c", "b", "a", "c")
x2 <- c(0.4, 0.42, 0.3, 0.39, 0.38, NA)

x <- matrix( c(x1, x2), nc = 2)
colnames(x) <- c("name", "pro")

# x contain two colume: horse(or jockey) and their pro
# output: return average pro, which stand for horse(or jockey) class
level <- function(x,colname) {

	pro <- data.frame(pro=matrix(as.numeric(x[,2]), nc=1))
	
	name <- c(x[,1])
	factor <- factor(name, exclude = "")
	
	rlt <- aggregate(pro, by=list(factor), FUN="median",na.rm=T)
	
	new.col <- matrix(NA, nr=nrow(x), nc=1)
	colnames(new.col) <- colname
	
	for (i in 1:nrow(rlt)) {
		ix <- x[,1] == rlt[i,1]
		new.col[ix,1] <- rlt[i,2]
	}	
	
	return( new.col)
}


level.historical <- function(x,colname) {

	pro <- data.frame(pro=matrix(as.numeric(x[,2]), nc=1))
	
	name <- c(x[,1])
	factor <- factor(name, exclude = "")
	
	rlt <- aggregate(pro, by=list(factor), FUN="median",na.rm=T)
	colnames(rlt) <- colname
	return(rlt)
}
a <- level.historical(x,colname = c("a","pro"))
a



# y contian two columes: horse(or jockey) name and historical average interval
# x contain two colume: horse(or jockey) name and empty colume which will contain historical average interval
# output: fill the historical average interval for every element in x and return

fill.avg.interval <- function(x, y) {

	for (i in 1:nrow(y)) {
		ix <- x[,1] == y[i,1]
		x[ix,2] <- y[i,2]
	}
	return(x)
}

# x contain two colume: horse(or jockey) and their finish time interval
# output: return average finish time interval
# n <- matrix(c("a","b","c","d","a","c") ,nc=1)
# v <- matrix(c( 2,  3,  3,  5,  1,  3.5), nc=1)
# x <- cbind(n,v)
# y <- interval.historical(x,c("name","interval"))
# View(y)
 
interval.historical <- function(x,colname) {

	int <- matrix(as.numeric(x[,2]), nc=1)
	
	interval <- data.frame(int)
	
	name <- c(x[,1])
	factor <- factor(name, exclude = "")
	
	rlt <- aggregate(interval, by=list(factor), FUN="median",na.rm=T)
	
	colnames(rlt) <- colname
	return(rlt)
}
