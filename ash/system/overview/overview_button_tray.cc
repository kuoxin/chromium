// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/system/overview/overview_button_tray.h"

#include "ash/shelf/shelf_types.h"
#include "ash/shell.h"
#include "ash/system/tray/system_tray_delegate.h"
#include "ash/system/tray/tray_utils.h"
#include "ash/wm/overview/window_selector_controller.h"
#include "grit/ash_resources.h"
#include "grit/ash_strings.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/views/border.h"
#include "ui/views/controls/image_view.h"

namespace {

// Predefined padding for the icon used in this tray. These are to be set to the
// border of the icon, depending on the current shelf_alignment()
const int kHorizontalShelfHorizontalPadding = 8;
const int kHorizontalShelfVerticalPadding = 4;
const int kVerticalShelfHorizontalPadding = 2;
const int kVerticalShelfVerticalPadding = 5;

}  // namespace

namespace ash {

OverviewButtonTray::OverviewButtonTray(
    internal::StatusAreaWidget* status_area_widget)
    : TrayBackgroundView(status_area_widget),
      icon_(NULL) {
  SetContentsBackground();

  ui::ResourceBundle& bundle = ui::ResourceBundle::GetSharedInstance();
  icon_ = new views::ImageView();
  icon_->SetImage(
      bundle.GetImageNamed(IDR_AURA_UBER_TRAY_OVERVIEW_MODE).ToImageSkia());
  SetIconBorderForShelfAlignment();
  tray_container()->AddChildView(icon_);

  UpdateIconVisibility(Shell::GetInstance()->
      IsMaximizeModeWindowManagerEnabled());

  Shell::GetInstance()->AddShellObserver(this);
}

OverviewButtonTray::~OverviewButtonTray() {
  Shell::GetInstance()->RemoveShellObserver(this);
}

void OverviewButtonTray::UpdateAfterLoginStatusChange(
    user::LoginStatus status) {
  UpdateIconVisibility(Shell::GetInstance()->
      IsMaximizeModeWindowManagerEnabled());
}

bool OverviewButtonTray::PerformAction(const ui::Event& event) {
  Shell::GetInstance()->window_selector_controller()->ToggleOverview();
  return true;
}

void OverviewButtonTray::OnMaximizeModeStarted() {
  // TODO(flackr): once maximize mode has been refactored remove this so that
  // UpdateIconVisibility polls Shell for the status directly
  UpdateIconVisibility(/* maximize_mode_enabled */ true);
}

void OverviewButtonTray::OnMaximizeModeEnded() {
  UpdateIconVisibility(/* maximize_mode_enabled */ false);
}

bool OverviewButtonTray::ClickedOutsideBubble() {
  // This class has no bubbles dismiss, but acknowledge that the message was
  // handled.
  return true;
}

base::string16 OverviewButtonTray::GetAccessibleNameForTray() {
  return l10n_util::GetStringUTF16(IDS_ASH_OVERVIEW_BUTTON_ACCESSIBLE_NAME);
}

void OverviewButtonTray::HideBubbleWithView(
    const views::TrayBubbleView* bubble_view) {
  // This class has no bubbles to hide.
}

void OverviewButtonTray::SetShelfAlignment(ShelfAlignment alignment) {
  if (alignment == shelf_alignment())
    return;

  TrayBackgroundView::SetShelfAlignment(alignment);
  SetIconBorderForShelfAlignment();
}

void OverviewButtonTray::SetIconBorderForShelfAlignment() {
  if (shelf_alignment() == SHELF_ALIGNMENT_BOTTOM ||
      shelf_alignment() == SHELF_ALIGNMENT_TOP) {
    icon_->SetBorder(views::Border::CreateEmptyBorder(
        kHorizontalShelfVerticalPadding,
        kHorizontalShelfHorizontalPadding,
        kHorizontalShelfVerticalPadding,
        kHorizontalShelfHorizontalPadding));
  } else {
    icon_->SetBorder(views::Border::CreateEmptyBorder(
        kVerticalShelfVerticalPadding,
        kVerticalShelfHorizontalPadding,
        kVerticalShelfVerticalPadding,
        kVerticalShelfHorizontalPadding));
  }
}

void OverviewButtonTray::UpdateIconVisibility(bool maximize_mode_enabled) {
  SetVisible(maximize_mode_enabled &&
             Shell::GetInstance()->window_selector_controller()->CanSelect());
}

}  // namespace ash
