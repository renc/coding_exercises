

# start trainning and predicting

interval.pro.pred <- matrix(NA, nrow(data.raw), nc=2)
colnames(interval.pro.pred) <- c("interval.pred","pro.pred")

sample.ix <- matrix(NA, nrow(data.raw), nc=1)
colnames(sample.ix) <- "last.row"

tot.cost <- 0

for(i in 1:length(race.id) * period) {

	if (i < 100) next

	ix.100 <- (i-1 > race.seq[,1]) & (i-100 < race.seq[,1])
	ix.50 <- (i-1 > race.seq[,1]) & (i-50 < race.seq[,1])
	ix.20 <- (i-1 > race.seq[,1]) & (i-20 < race.seq[,1])
	
	# 记录样本时间，避免使用到未来数据
	for (j in nrow(data.raw):1) {
		if (race.seq[j,1] == i) {
			for (k in nrow(data.raw):1) {			
				if (ix.100[k]) {			
					sample.ix[j] <- race.seq[k,1]
					break;
			}	}
			break;
	}	}	
	
	
	# Y, 因变量
	y <- interval[ix.100,1]
	#y <- y - median(y,na.r=T)


	# factor1:horse level
	x.100 <- x.50 <- x.20 <- matrix( NA, nr=sum(ix.100,na.rm=T), nc=1 )
	x.100 <- matrix(avg.horse.int.100[ix.100,1],nc=1); colnames(x.100) <- "avg.horse.int.100"
	x.50 <- matrix(avg.horse.int.50[ix.100,1],nc=1); colnames(x.50) <- "avg.horse.int.50"
	x.20 <- matrix(avg.horse.int.20[ix.100,1],nc=1); colnames(x.20) <- "avg.horse.int.20"
	
	x1 <- x.20 - x.50
	
	# factor2:jockey level
	x.100 <- x.50 <- x.20 <- matrix( NA, nr=sum(ix.100,na.rm=T), nc=1 )
	x.100 <- matrix(avg.jockey.int.100[ix.100,1],nc=1); colnames(x.100) <- "avg.jockey.int.100"
	x.50 <- matrix(avg.jockey.int.50[ix.100,1],nc=1); colnames(x.50) <- "avg.jockey.int.50"
	x.20 <- matrix(avg.jockey.int.20[ix.100,1],nc=1); colnames(x.20) <- "avg.jockey.int.20"
	
	x2 <- x.20 - x.50
	
	# factor2:horse level (50 races)
	x <- matrix( data.raw[ix, horse.ix] )
	#x2 <- level(cbind(x,pro[ix,1]), "horse.pro")
	#x2 <- interval.fun(cbind(x,matrix(interval[ix,1],nc=1)), "horse.pro")

	xs <- as.data.frame(cbind(x1,x2))
	out.ls=glm(y~.,data = xs)	
	#  summary(out.ls)


########## predict part ########## 
	ix.pred <- i ==  race.seq[,1]
	
	# factor1:horse level
	x.now.100 <- x.now.50 <- x.now.20 <-  matrix( NA, nr=sum(ix.pred,na.rm=T), nc=1 )
	x.now.100 <- matrix(avg.horse.int.100[ix.pred,1],nc=1); colnames(x.now.100) <- "avg.horse.int.100"
	x.now.50 <- matrix(avg.horse.int.50[ix.pred,1],nc=1); colnames(x.now.50) <-  "avg.horse.int.50"
	x.now.20 <- matrix(avg.horse.int.20[ix.pred,1],nc=1); colnames(x.now.20) <-  "avg.horse.int.20"
	x.now.1 <- x.now.20 - x.now.50
	
	# factor2:jockey level
	x.now.100 <- x.now.50 <- x.now.20 <-  matrix( NA, nr=sum(ix.pred,na.rm=T), nc=1 )
	x.now.100 <- matrix(avg.jockey.int.100[ix.pred,1],nc=1); colnames(x.now.100) <- "avg.jockey.int.100"
	x.now.50 <- matrix(avg.jockey.int.50[ix.pred,1],nc=1); colnames(x.now.50) <-  "avg.jockey.int.50"
	x.now.20 <- matrix(avg.jockey.int.20[ix.pred,1],nc=1); colnames(x.now.20) <-  "avg.jockey.int.20"
	x.now.2 <- x.now.20 - x.now.50
	
	xs.pred <- as.data.frame(cbind(x.now.1,x.now.2))	#,x.now.20))
	interval.pred <- predict(out.ls, newdata=xs.pred)
	interval.pred[interval.pred<0] <- 0

	interval.pred <- as.matrix(interval.pred, nc=1)
	#	cat(i,":",interval.pred,"\n")
	
	pro.pred <- interval.2.pro(interval.pred, shrunk = 1000)
	interval.pro.pred[ix.pred,1] <- interval.pred
	interval.pro.pred[ix.pred,2] <- pro.pred
	
}

data.w.pred <- cbind(data.raw, interval.pro.pred)


# optimaze betting
tot.cost <- 0
bet.profit <- matrix(0, nrow(data.raw), nc=2)
colnames(bet.profit) <- c("bet","profit")

for(i in 100:length(race.id) * period) {

	ix.pred <- i ==  race.seq[,1]
	
	odd.i <- matrix(as.numeric(data.raw[ix.pred, odd.ix]),nc=1)
	interval.pred <- matrix(as.numeric(interval.pro.pred[ix.pred,1]),nc=1)
	interval.pred.order <- sort(interval.pred,dec=T)
	
	pro.pred <- matrix(as.numeric(interval.pro.pred[ix.pred,2]),nc=1)
	pro.pred.order <- sort(pro.pred,dec=T)
	
	odd.pro <- odd.i * matrix(as.numeric(interval.pro.pred[ix.pred,2]),nc=1)
	odd.pro.order <- sort(odd.pro,dec=T)
	
	q <- quantile(pro.pred,na.rm=T)
	#odd.pro[pro.pred < pro.pred.order[5] | pro.pred > pro.pred.order[1]] <- NA
	
	q <- quantile(odd.pro,na.rm=T)
	odd.pro[odd.pro < odd.pro.order[4]] <- NA		# 计算五分位数，将小于中值的设为NA
	#odd.pro[odd.pro < 5] <- NA
	
	if ( sum(odd.pro, na.rm=T) > 30) 
	{
		#bet <- odd.pro / sum(odd.pro,na.rm=T)
		bet <- (max(interval.pred,na.rm=T) - interval.pred) / sum((max(interval.pred,na.rm=T) - interval.pred),na.rm=T)
		
		bet[bet > 0.5] <- 0.5
		tot.cost <- tot.cost - sum(abs(bet),na.rm=T)
		bet.profit[ix.pred,1] <- bet
	}	
}

ix.first <- data.raw[,finishing.position.ix] == "1"
bet.profit[ix.first,2] <- (as.numeric(data.raw[ix.first,odd.ix])) * as.numeric(bet.profit[ix.first,1])

cat("tot.cost:",tot.cost,"\n")
cat("tot.prof:",sum(bet.profit[,2],na.rm=T),"\n")


write.table(cbind(data.w.pred, sample.ix, race.seq, bet.profit), paste("./output/result.csv", sep=""), sep=",",row.names=FALSE)

