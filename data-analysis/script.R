library(psych)
library(ggplot2)

Hp = mtcars$hp
Mpg = mtcars$mpg
Cyl = mtcars$cyl

MpgName = "Miles Per Gallon"
HpName = "Horsepower"
CylName = "Number of Cylinders"

###
mode <- function(v) {
  uniqv <- unique(v)
  uniqv[which.max(tabulate(match(v, uniqv)))]
}

#min, 1st qurtile, meadian, mean, 3rd quartile, max
summary(Hp)
summary(Mpg)
#mode
mode(Hp)
mode(Mpg)
#variance
var(Hp)
var(Mpg)
#standard deviation
sd(Hp)
sd(Mpg)
#coefficient of variation
sd(Hp)/mean(Hp)*100
sd(Mpg)/mean(Mpg)*100
#range
max(Hp)-min(Hp)
max(Mpg)-min(Mpg)
#geometric mean
geometric.mean(Hp)
geometric.mean(Mpg)
#harmonic mean
harmonic.mean(Hp)
harmonic.mean(Mpg)

#scatter plot
plot(Hp, Mpg, xlab = HpName, ylab = MpgName)
pairs(~mpg+hp+cyl,data = mtcars)
#stem-and-leaf plot
#stem(Hp)
#stem(Mpg)
#box-and-whisker plot
boxplot(Mpg, horizontal = TRUE, xlab = MpgName)$out
boxplot(Cyl, horizontal = TRUE, xlab = CylName)$out
anomalies <- boxplot(Hp, horizontal = TRUE, xlab = HpName)$out
anomalies

#Shapiro-Wilk’s normality test
shapiro.test(Mpg)
shapiro.test(Hp)

###
#regression function
model <- lm(Mpg ~ Hp)
#print(coefficients(model))
#coefficient of determination
summary(model)$r.squared

plot(Hp, Mpg, main = paste(MpgName, "&", HpName, "Regression", sep=" "),
     abline(lm(Mpg ~ Hp)),pch = 16,xlab = HpName,ylab = MpgName)

###
#coefficient of correlation
cor(Mpg, Hp)

#pearson's correlation
cor.test(~Mpg + Hp,
         method = "pearson",
         continuity = FALSE,
         conf.level = 0.95)
#spearman's rank correlation
cor.test(~Mpg + Hp,
          method = "spearman",
          continuity = FALSE,
          conf.level = 0.95)
#kendall's rank correlation
cor.test(~Mpg + Hp,
         method = "kendall",
         continuity = FALSE,
         conf.level = 0.95)

#contingency table
table(Cyl, mtcars$vs)
