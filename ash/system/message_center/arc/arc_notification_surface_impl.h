// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_MESSAGE_CENTER_ARC_ARC_NOTIFICATION_SURFACE_IMPL_H_
#define ASH_SYSTEM_MESSAGE_CENTER_ARC_ARC_NOTIFICATION_SURFACE_IMPL_H_

#include <memory>

#include "ash/system/message_center/arc/arc_notification_surface.h"

namespace exo {
class NotificationSurface;
}

namespace ash {

// Handles notification surface role of a given surface.
class ArcNotificationSurfaceImpl : public ArcNotificationSurface {
 public:
  explicit ArcNotificationSurfaceImpl(exo::NotificationSurface* surface);
  ~ArcNotificationSurfaceImpl() override;

  // ArcNotificationSurface overrides:
  gfx::Size GetSize() const override;
  aura::Window* GetWindow() const override;
  aura::Window* GetContentWindow() const override;
  const std::string& GetNotificationKey() const override;
  void Attach(views::NativeViewHost* native_view_host) override;
  void Detach() override;
  bool IsAttached() const override;
  views::NativeViewHost* GetAttachedHost() const override;
  void FocusSurfaceWindow() override;
  void SetAXTreeId(int32_t ax_tree_id) override;
  int32_t GetAXTreeId() const override;

  exo::NotificationSurface* surface() const { return surface_; }

 private:
  exo::NotificationSurface* surface_;
  views::NativeViewHost* native_view_host_ = nullptr;
  std::unique_ptr<aura::Window> native_view_;
  int32_t ax_tree_id_ = -1;

  DISALLOW_COPY_AND_ASSIGN(ArcNotificationSurfaceImpl);
};

}  // namespace ash

#endif  // ASH_SYSTEM_MESSAGE_CENTER_ARC_ARC_NOTIFICATION_SURFACE_IMPL_H_
