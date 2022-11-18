﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/shield/Shield_EXPORTS.h>
#include <aws/shield/ShieldRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Shield
{
namespace Model
{

  /**
   */
  class AWS_SHIELD_API DescribeProtectionGroupRequest : public ShieldRequest
  {
  public:
    DescribeProtectionGroupRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DescribeProtectionGroupRequest> Clone() const
    {
      return Aws::MakeUnique<DescribeProtectionGroupRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DescribeProtectionGroup"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The name of the protection group. You use this to identify the protection
     * group in lists and to manage the protection group, for example to update,
     * delete, or describe it. </p>
     */
    inline const Aws::String& GetProtectionGroupId() const{ return m_protectionGroupId; }

    /**
     * <p>The name of the protection group. You use this to identify the protection
     * group in lists and to manage the protection group, for example to update,
     * delete, or describe it. </p>
     */
    inline bool ProtectionGroupIdHasBeenSet() const { return m_protectionGroupIdHasBeenSet; }

    /**
     * <p>The name of the protection group. You use this to identify the protection
     * group in lists and to manage the protection group, for example to update,
     * delete, or describe it. </p>
     */
    inline void SetProtectionGroupId(const Aws::String& value) { m_protectionGroupIdHasBeenSet = true; m_protectionGroupId = value; }

    /**
     * <p>The name of the protection group. You use this to identify the protection
     * group in lists and to manage the protection group, for example to update,
     * delete, or describe it. </p>
     */
    inline void SetProtectionGroupId(Aws::String&& value) { m_protectionGroupIdHasBeenSet = true; m_protectionGroupId = std::move(value); }

    /**
     * <p>The name of the protection group. You use this to identify the protection
     * group in lists and to manage the protection group, for example to update,
     * delete, or describe it. </p>
     */
    inline void SetProtectionGroupId(const char* value) { m_protectionGroupIdHasBeenSet = true; m_protectionGroupId.assign(value); }

    /**
     * <p>The name of the protection group. You use this to identify the protection
     * group in lists and to manage the protection group, for example to update,
     * delete, or describe it. </p>
     */
    inline DescribeProtectionGroupRequest& WithProtectionGroupId(const Aws::String& value) { SetProtectionGroupId(value); return *this;}

    /**
     * <p>The name of the protection group. You use this to identify the protection
     * group in lists and to manage the protection group, for example to update,
     * delete, or describe it. </p>
     */
    inline DescribeProtectionGroupRequest& WithProtectionGroupId(Aws::String&& value) { SetProtectionGroupId(std::move(value)); return *this;}

    /**
     * <p>The name of the protection group. You use this to identify the protection
     * group in lists and to manage the protection group, for example to update,
     * delete, or describe it. </p>
     */
    inline DescribeProtectionGroupRequest& WithProtectionGroupId(const char* value) { SetProtectionGroupId(value); return *this;}

  private:

    Aws::String m_protectionGroupId;
    bool m_protectionGroupIdHasBeenSet = false;
  };

} // namespace Model
} // namespace Shield
} // namespace Aws
