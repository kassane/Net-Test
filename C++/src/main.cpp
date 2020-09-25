#include <iostream>
#include <stx/panic.h>
#include <stx/option.h>
#include <asio/execution.hpp>
#include <asio/static_thread_pool.hpp>

using asio::static_thread_pool;
namespace execution = asio::execution;
using stx::Option, stx::Some, stx::None;

// Traditional active object pattern.
// Member functions do not block.

class bank_account
{
  int balance_ = 0;
  mutable static_thread_pool pool_{1};

public:
  void deposit(int amount)
  {
    execution::execute(
        asio::require(pool_.executor(),
          execution::blocking.always),
        [this, amount]
        {
          balance_ += amount;
        });
  }

  void withdraw(int amount)
  {
    execution::execute(
        asio::require(pool_.executor(),
          execution::blocking.always),
        [this, amount]
        {
          if (balance_ >= amount)
            balance_ -= amount;
          else if(balance_ < amount) stx::panic("No withdraw!");
        });
  }

  auto balance() const -> Option<int>
  {
      int result = 0;
    execution::execute(
        asio::require(pool_.executor(),
          execution::blocking.always),
        [this, &result]
        {
          result = balance_;
        });
    if (result <= 0)
        return None;
    return Some(result/1);
  }
};

int main()
{
  bank_account acct;
  acct.deposit(20);
  acct.withdraw(100);
  //std::cout << "balance = " << acct.balance() << "\n";
  acct.balance().match(
      [](auto value) { std::cout << "Result: " << value << std::endl; },
      []() { std::cout << "Cannot divide by zero" << std::endl; });
}
