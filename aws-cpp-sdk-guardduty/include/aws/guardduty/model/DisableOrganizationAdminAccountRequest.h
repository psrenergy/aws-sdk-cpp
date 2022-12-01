﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/guardduty/GuardDuty_EXPORTS.h>
#include <aws/guardduty/GuardDutyRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace GuardDuty
{
namespace Model
{

  /**
   */
  class AWS_GUARDDUTY_API DisableOrganizationAdminAccountRequest : public GuardDutyRequest
  {
  public:
    DisableOrganizationAdminAccountRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DisableOrganizationAdminAccountRequest> Clone() const
    {
      return Aws::MakeUnique<DisableOrganizationAdminAccountRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DisableOrganizationAdminAccount"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The Amazon Web Services Account ID for the organizations account to be
     * disabled as a GuardDuty delegated administrator.</p>
     */
    inline const Aws::String& GetAdminAccountId() const{ return m_adminAccountId; }

    /**
     * <p>The Amazon Web Services Account ID for the organizations account to be
     * disabled as a GuardDuty delegated administrator.</p>
     */
    inline bool AdminAccountIdHasBeenSet() const { return m_adminAccountIdHasBeenSet; }

    /**
     * <p>The Amazon Web Services Account ID for the organizations account to be
     * disabled as a GuardDuty delegated administrator.</p>
     */
    inline void SetAdminAccountId(const Aws::String& value) { m_adminAccountIdHasBeenSet = true; m_adminAccountId = value; }

    /**
     * <p>The Amazon Web Services Account ID for the organizations account to be
     * disabled as a GuardDuty delegated administrator.</p>
     */
    inline void SetAdminAccountId(Aws::String&& value) { m_adminAccountIdHasBeenSet = true; m_adminAccountId = std::move(value); }

    /**
     * <p>The Amazon Web Services Account ID for the organizations account to be
     * disabled as a GuardDuty delegated administrator.</p>
     */
    inline void SetAdminAccountId(const char* value) { m_adminAccountIdHasBeenSet = true; m_adminAccountId.assign(value); }

    /**
     * <p>The Amazon Web Services Account ID for the organizations account to be
     * disabled as a GuardDuty delegated administrator.</p>
     */
    inline DisableOrganizationAdminAccountRequest& WithAdminAccountId(const Aws::String& value) { SetAdminAccountId(value); return *this;}

    /**
     * <p>The Amazon Web Services Account ID for the organizations account to be
     * disabled as a GuardDuty delegated administrator.</p>
     */
    inline DisableOrganizationAdminAccountRequest& WithAdminAccountId(Aws::String&& value) { SetAdminAccountId(std::move(value)); return *this;}

    /**
     * <p>The Amazon Web Services Account ID for the organizations account to be
     * disabled as a GuardDuty delegated administrator.</p>
     */
    inline DisableOrganizationAdminAccountRequest& WithAdminAccountId(const char* value) { SetAdminAccountId(value); return *this;}

  private:

    Aws::String m_adminAccountId;
    bool m_adminAccountIdHasBeenSet = false;
  };

} // namespace Model
} // namespace GuardDuty
} // namespace Aws
