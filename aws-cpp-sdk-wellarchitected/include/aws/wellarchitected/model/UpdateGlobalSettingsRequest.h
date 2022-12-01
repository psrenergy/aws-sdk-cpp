﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/wellarchitected/WellArchitected_EXPORTS.h>
#include <aws/wellarchitected/WellArchitectedRequest.h>
#include <aws/wellarchitected/model/OrganizationSharingStatus.h>
#include <utility>

namespace Aws
{
namespace WellArchitected
{
namespace Model
{

  /**
   */
  class AWS_WELLARCHITECTED_API UpdateGlobalSettingsRequest : public WellArchitectedRequest
  {
  public:
    UpdateGlobalSettingsRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<UpdateGlobalSettingsRequest> Clone() const
    {
      return Aws::MakeUnique<UpdateGlobalSettingsRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "UpdateGlobalSettings"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The status of organization sharing settings.</p>
     */
    inline const OrganizationSharingStatus& GetOrganizationSharingStatus() const{ return m_organizationSharingStatus; }

    /**
     * <p>The status of organization sharing settings.</p>
     */
    inline bool OrganizationSharingStatusHasBeenSet() const { return m_organizationSharingStatusHasBeenSet; }

    /**
     * <p>The status of organization sharing settings.</p>
     */
    inline void SetOrganizationSharingStatus(const OrganizationSharingStatus& value) { m_organizationSharingStatusHasBeenSet = true; m_organizationSharingStatus = value; }

    /**
     * <p>The status of organization sharing settings.</p>
     */
    inline void SetOrganizationSharingStatus(OrganizationSharingStatus&& value) { m_organizationSharingStatusHasBeenSet = true; m_organizationSharingStatus = std::move(value); }

    /**
     * <p>The status of organization sharing settings.</p>
     */
    inline UpdateGlobalSettingsRequest& WithOrganizationSharingStatus(const OrganizationSharingStatus& value) { SetOrganizationSharingStatus(value); return *this;}

    /**
     * <p>The status of organization sharing settings.</p>
     */
    inline UpdateGlobalSettingsRequest& WithOrganizationSharingStatus(OrganizationSharingStatus&& value) { SetOrganizationSharingStatus(std::move(value)); return *this;}

  private:

    OrganizationSharingStatus m_organizationSharingStatus;
    bool m_organizationSharingStatusHasBeenSet = false;
  };

} // namespace Model
} // namespace WellArchitected
} // namespace Aws
