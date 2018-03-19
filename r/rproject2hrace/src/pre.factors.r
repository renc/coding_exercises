
# 1)
# get raw data from file
#data.raw <- as.matrix(read.csv(paste("../input/race-result-horse-fromKaggle.LantanaCamara.2014to2016.csv", sep=""), sep=",", header=T))
# renc, to use a small sub set date for test. 
#data.raw <- as.matrix(read.csv(paste("../input/race-result-horse-fromKaggle.LantanaCamara.2016.019.csv", sep = ""), sep = ",", header = T))
data.raw <- as.matrix(read.csv(paste("../input/race-result-horse-fromKaggle.LantanaCamara.2016.110.csv", sep = ""), sep = ",", header = T))


ix <- matrix(TRUE, nr=nrow(data.raw), nc=1)
first.col <- as.numeric(data.raw[,1]) #renc, Warning message: NAs introduced by coercion
ix <- first.col > 20
ix[ix == FALSE] <- TRUE
ix[is.na(ix)] <- FALSE
# renc, after the above, the horse finish the race will be TRUE, does not finish the race will be FALSE
data.raw <- data.raw[ix,]
# renc, looks like the horses do not finish the race are ignored/removed.
data.raw[,race.ix] 

# 2)
### get unique race id
race.id <- unique(data.raw[, race.ix], incomparables = FALSE)
View(race.id)
# renc, there could be a bug here, when the race.id is not sorted (ascending). 
race.seq <- matrix(NA,nr = nrow(data.raw),nc=1)
colnames(race.seq) <- "race.num"
for (i in 1:length(race.id)) {
	ix <- data.raw[,race.ix] == race.id[i]
	race.seq[ix,1] <- i
} # renc, the values at race.seq[ix, 1] the first column, are set to unique i. 
View(race.seq)


# 3)
# turn interval into pro for each race   # renc, turn the (finish time) into probability ?
pro <- matrix(NA, nrow(data.raw), nc=1)
colnames(pro) <- "pro"
for (i in 1:length(race.id)) {
	ix <- data.raw[,race.ix] == race.id[i]
	pro[ix,1] <- time.2.pro(matrix(data.raw[ix,interval.ix]), shrunk = 1000)
	#pro[ix,1]
}
View(pro)
#data.raw <- cbind(data.raw, pro)

# 4) 
# turn time to interval
interval <- matrix(NA, nrow(data.raw), nc=1)
colnames(pro) <- "interval"                     #renc, this is bug? colnames(interval) <- "interval"  
for (i in 1:length(race.id)) {
	ix <- data.raw[,race.ix] == race.id[i]
	interval[ix,1] <- time.2.interval(matrix(data.raw[ix,interval.ix]))
	#interval[ix,1]
}
View(interval)
#data.raw <- cbind(data.raw, pro, interval)
#View(data.raw)


# 5) caculate historical horse interval:
avg.horse.int.100 <- matrix(NA,nrow(data.raw),nc=1)	#过去100场平均时间  # renc, of each horse ? or of each race ?
avg.horse.int.50 <- matrix(NA,nrow(data.raw),nc=1)
avg.horse.int.20 <- matrix(NA,nrow(data.raw),nc=1)
for(i in 1:length(race.id)) {

	if (i < start.date) next
    #if (FALSE)
    {
        # 100
        ix.100 <- (i - 1 >= race.seq[, 1]) & (i - roll.back.horse.interval.1 < race.seq[, 1]) # renc, the mask of race.seq[] to get the last 100 race
        horse <- data.raw[ix.100, horse.ix]
        interval.slice <- interval[ix.100, 1]
        avg.100 <- interval.historical(cbind(horse, interval.slice), c("horse", "interval"))

        ix <- i == race.seq[, 1]   # renc, the mask of race.seq for the current race.
        x <- matrix(NA, nr = sum(ix, na.rm = T), nc = 2) #x contain two colume:horse(or jockey) name and empty colume which will contain historical average interval
        x[, 1] <- data.raw[ix, c(horse.ix)]

        x <- fill.avg.interval(x, avg.100)
        avg.horse.int.100[ix, 1] <- as.numeric(x[, 2])

        # 50
        ix.50 <- (i - 1 >= race.seq[, 1]) & (i - roll.back.horse.interval.2 < race.seq[, 1])
        horse <- data.raw[ix.50, horse.ix]
        interval.slice <- interval[ix.50, 1]
        avg.50 <- interval.historical(cbind(horse, interval.slice), c("horse", "interval"))

        ix <- i == race.seq[, 1]
        x <- matrix(NA, nr = sum(ix, na.rm = T), nc = 2)
        x[, 1] <- data.raw[ix, c(horse.ix)]

        x <- fill.avg.interval(x, avg.50)
        avg.horse.int.50[ix, 1] <- as.numeric(x[, 2])
    }
	# 20
    ix.20 <- (i - 1 >= race.seq[, 1]) & (i - roll.back.horse.interval.3 < race.seq[, 1])
    #print(ix.20) # renc, for debug

	horse <- data.raw[ix.20,horse.ix]
	interval.slice <- interval[ix.20,1]
	avg.20 <- interval.historical( cbind(horse,interval.slice),c("horse","interval"))

	ix <- i ==  race.seq[,1]
	x <- matrix(NA, nr=sum(ix,na.rm=T), nc=2)
	x[,1] <- data.raw[ix,c(horse.ix)]

	x <- fill.avg.interval(x, avg.20)
	avg.horse.int.20[ix,1] <- as.numeric(x[,2])
}
View(cbind(avg.horse.int.100,avg.horse.int.50,avg.horse.int.20))

# 6) caculate historical jockey interval:
avg.jockey.int.100 <- matrix(NA,nrow(data.raw),nc=1)	#过去100场平均时间
avg.jockey.int.50 <- matrix(NA,nrow(data.raw),nc=1)
avg.jockey.int.20 <- matrix(NA,nrow(data.raw),nc=1)
for(i in 1:length(race.id)) {

	if (i < start.date) next
	
	# 100
	ix.100 <- (i-1 >= race.seq[,1]) & (i-roll.back.jockey.interval.1 < race.seq[,1])
	jockey <- data.raw[ix.100,jockey.ix]
	interval.slice <- interval[ix.100,1]
	avg.100 <- interval.historical( cbind(jockey,interval.slice),c("jockey","interval"))

	ix <- i ==  race.seq[,1]
	x <- matrix(NA, nr=sum(ix,na.rm=T), nc=2)
	x[,1] <- data.raw[ix,c(jockey.ix)]
	
	x <- fill.avg.interval(x, avg.100)
	avg.jockey.int.100[ix,1] <- as.numeric(x[,2])
	
	# 50
	ix.50 <- (i-1 >= race.seq[,1]) & (i-roll.back.jockey.interval.2 < race.seq[,1])
	jockey <- data.raw[ix.50,jockey.ix]
	interval.slice <- interval[ix.50,1]
	avg.50 <- interval.historical( cbind(jockey,interval.slice),c("jockey","interval"))

	ix <- i ==  race.seq[,1]
	x <- matrix(NA, nr=sum(ix,na.rm=T), nc=2)
	x[,1] <- data.raw[ix,c(jockey.ix)]

	x <- fill.avg.interval(x, avg.50)
	avg.jockey.int.50[ix,1] <- as.numeric(x[,2])

	# 20
	ix.20 <- (i-1 >= race.seq[,1]) & (i-roll.back.jockey.interval.3 < race.seq[,1])
	jockey <- data.raw[ix.20,jockey.ix]
	interval.slice <- interval[ix.20,1]
	avg.20 <- interval.historical( cbind(jockey,interval.slice),c("jockey","interval"))

	ix <- i ==  race.seq[,1]
	x <- matrix(NA, nr=sum(ix,na.rm=T), nc=2)
	x[,1] <- data.raw[ix,c(jockey.ix)]

	x <- fill.avg.interval(x, avg.20)
	avg.jockey.int.20[ix,1] <- as.numeric(x[,2])
}
View(cbind(avg.jockey.int.100,avg.jockey.int.50,avg.jockey.int.20))

