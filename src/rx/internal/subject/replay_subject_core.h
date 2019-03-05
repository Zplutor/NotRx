#pragma once

#include <optional>
#include "rx/internal/subject/subject_core.h"

namespace rx {
namespace internal {

class ReplaySubjectCore : public SubjectCore {
protected:
    Work OnSubscribeExtension(const std::shared_ptr<Observer>& observer) override;
    void OnNextExtension(const std::any& value) override;
    void OnErrorExtension(const Error& error) override;
    void OnCompletedExtension() override;

private:
    std::vector<std::any> values_;
    std::optional<Error> error_;
    bool is_completed_{};
};

}
}