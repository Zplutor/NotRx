#include "rx/internal/subject.h"
#include <algorithm>

namespace rx {
namespace internal {

class SubjectImpl : public Observable, public Observer {
public:
    std::shared_ptr<Subscription> Subscribe(const std::shared_ptr<Observer>& observer) override {
        observers_.push_back(observer);
        return std::make_shared<SubjectSubscription>(std::dynamic_pointer_cast<SubjectImpl>(shared_from_this()), observer);
    }

    void OnNext(const std::any& value) override {
        for (const auto& each_observer : observers_) {
            each_observer->OnNext(value);
        }
    }

    void OnError(const Error& error) override {
        for (const auto& each_observer : observers_) {
            each_observer->OnError(error);
        }
    }

    void OnCompleted() override {
        for (const auto& each_observer : observers_) {
            each_observer->OnCompleted();
        }
    }

private:
    class SubjectSubscription : public Subscription {
    public:
        SubjectSubscription(std::shared_ptr<SubjectImpl> subject, std::shared_ptr<Observer> observer) : 
            subject_(std::move(subject)),
            observer_(std::move(observer)) {

        }

        void Unsubscribe() override {
            auto subject = subject_.lock();
            if (subject != nullptr) {
                subject->RemoveObserver(observer_);
            }
        }

    private:
        std::weak_ptr<SubjectImpl> subject_;
        std::shared_ptr<Observer> observer_;
    };

private:
    void RemoveObserver(const std::shared_ptr<Observer>& observer) {
        auto end = std::remove(observers_.begin(), observers_.end(), observer);
        observers_.erase(end, observers_.end());
    }

private:
    std::vector<std::shared_ptr<Observer>> observers_;
};


std::shared_ptr<Subject> Subject::Create() {
    return std::make_shared<Subject>();
}


Subject::Subject() : impl_(std::make_shared<SubjectImpl>()) {

}


Subject::~Subject() {

}


std::shared_ptr<Observable> Subject::GetObservable() {
    return impl_;
}


std::shared_ptr<Observer> Subject::GetObserver() {
    return impl_;
}

}
}