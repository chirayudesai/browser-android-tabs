// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/task_runner_util.h"

#include <utility>

#include "base/bind.h"
#include "base/location.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

namespace {

int ReturnFourtyTwo() {
  return 42;
}

void StoreValue(int* destination, int value) {
  *destination = value;
}

void StoreDoubleValue(double* destination, double value) {
  *destination = value;
}

int g_foo_destruct_count = 0;
int g_foo_free_count = 0;

struct Foo {
  ~Foo() {
    ++g_foo_destruct_count;
  }
};

std::unique_ptr<Foo> CreateFoo() {
  return std::unique_ptr<Foo>(new Foo);
}

void ExpectFoo(std::unique_ptr<Foo> foo) {
  EXPECT_TRUE(foo.get());
  std::unique_ptr<Foo> local_foo(std::move(foo));
  EXPECT_TRUE(local_foo.get());
  EXPECT_FALSE(foo.get());
}

struct FooDeleter {
  void operator()(Foo* foo) const {
    ++g_foo_free_count;
    delete foo;
  };
};

std::unique_ptr<Foo, FooDeleter> CreateScopedFoo() {
  return std::unique_ptr<Foo, FooDeleter>(new Foo);
}

void ExpectScopedFoo(std::unique_ptr<Foo, FooDeleter> foo) {
  EXPECT_TRUE(foo.get());
  std::unique_ptr<Foo, FooDeleter> local_foo(std::move(foo));
  EXPECT_TRUE(local_foo.get());
  EXPECT_FALSE(foo.get());
}

struct FooWithoutDefaultConstructor {
  explicit FooWithoutDefaultConstructor(int value) : value(value) {}
  int value;
};

FooWithoutDefaultConstructor CreateFooWithoutDefaultConstructor(int value) {
  return FooWithoutDefaultConstructor(value);
}

void SaveFooWithoutDefaultConstructor(int* output_value,
                                      FooWithoutDefaultConstructor input) {
  *output_value = input.value;
}

}  // namespace

TEST(TaskRunnerHelpersTest, PostTaskAndReplyWithResult) {
  int result = 0;

  MessageLoop message_loop;
  PostTaskAndReplyWithResult(message_loop.task_runner().get(), FROM_HERE,
                             BindOnce(&ReturnFourtyTwo),
                             BindOnce(&StoreValue, &result));

  RunLoop().RunUntilIdle();

  EXPECT_EQ(42, result);
}

TEST(TaskRunnerHelpersTest, PostTaskAndReplyWithResultImplicitConvert) {
  double result = 0;

  MessageLoop message_loop;
  PostTaskAndReplyWithResult(message_loop.task_runner().get(), FROM_HERE,
                             BindOnce(&ReturnFourtyTwo),
                             BindOnce(&StoreDoubleValue, &result));

  RunLoop().RunUntilIdle();

  EXPECT_DOUBLE_EQ(42.0, result);
}

TEST(TaskRunnerHelpersTest, PostTaskAndReplyWithResultPassed) {
  g_foo_destruct_count = 0;
  g_foo_free_count = 0;

  MessageLoop message_loop;
  PostTaskAndReplyWithResult(message_loop.task_runner().get(), FROM_HERE,
                             BindOnce(&CreateFoo), BindOnce(&ExpectFoo));

  RunLoop().RunUntilIdle();

  EXPECT_EQ(1, g_foo_destruct_count);
  EXPECT_EQ(0, g_foo_free_count);
}

TEST(TaskRunnerHelpersTest, PostTaskAndReplyWithResultPassedFreeProc) {
  g_foo_destruct_count = 0;
  g_foo_free_count = 0;

  MessageLoop message_loop;
  PostTaskAndReplyWithResult(message_loop.task_runner().get(), FROM_HERE,
                             BindOnce(&CreateScopedFoo),
                             BindOnce(&ExpectScopedFoo));

  RunLoop().RunUntilIdle();

  EXPECT_EQ(1, g_foo_destruct_count);
  EXPECT_EQ(1, g_foo_free_count);
}

TEST(TaskRunnerHelpersTest,
     PostTaskAndReplyWithResultWithoutDefaultConstructor) {
  const int kSomeVal = 17;

  MessageLoop message_loop;
  int actual = 0;
  PostTaskAndReplyWithResult(
      message_loop.task_runner().get(), FROM_HERE,
      BindOnce(&CreateFooWithoutDefaultConstructor, kSomeVal),
      BindOnce(&SaveFooWithoutDefaultConstructor, &actual));

  RunLoop().RunUntilIdle();

  EXPECT_EQ(kSomeVal, actual);
}

}  // namespace base
