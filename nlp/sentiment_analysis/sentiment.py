import re
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score
from sklearn.model_selection import train_test_split

reviews_train = []
for line in open('movie_data/full_train.txt', 'r'):
    reviews_train.append(line.strip())

reviews_test = []
for line in open('movie_data/full_test.txt', 'r'):
    reviews_test.append(line.strip())

REMOVE = re.compile("(\.)|(;)|(:)|(!)|(\')|(\?)|(,)|(\")|(\()|(\))|(\[)|(\])")
REPLACE_WITH_SPACE = re.compile("(<br\s*/><br\s*/>)|(-)|(/)")


def preprocess_reviews(reviews):
    reviews = [REMOVE.sub("", line.lower()) for line in reviews]
    reviews = [REPLACE_WITH_SPACE.sub(" ", line) for line in reviews]

    return reviews


print(reviews_train[0])

reviews_train_clean = preprocess_reviews(reviews_train)
reviews_test_clean = preprocess_reviews(reviews_test)

print(reviews_train_clean[0])

cv = CountVectorizer(binary=True)
cv.fit(reviews_train_clean)
X = cv.transform(reviews_train_clean)
X_test = cv.transform(reviews_test_clean)

print(X)

target = [1 if i < 12500 else 0 for i in range(25000)]  # 12500 pos, 12500 neg

X_train, X_val, y_train, y_val = train_test_split(X, target, train_size=0.75)

for c in [0.01, 0.05, 0.25, 0.5, 1]:
    lr = LogisticRegression(C=c)
    lr.fit(X_train, y_train)
    acc = accuracy_score(y_val, lr.predict(X_val))
    print("Accuracy for C={}: {}".format(c, acc))

final_model = LogisticRegression(C=0.05)
final_model.fit(X, target)
print("Final Accuracy: {}".format(accuracy_score(target, final_model.predict(X_test))))

print(len(final_model.coef_[0]), final_model.coef_)

feature_to_coef = {word: coef for word, coef in zip(cv.get_feature_names(), final_model.coef_[0])}

print("Positive:")
for best_positive in sorted(
        feature_to_coef.items(),
        key=lambda x: x[1],
        reverse=True)[:5]:
    print(best_positive)

print("Negative:")
for best_negative in sorted(
        feature_to_coef.items(),
        key=lambda x: x[1])[:5]:
    print(best_negative)
