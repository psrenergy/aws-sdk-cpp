﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/greengrass/Greengrass_EXPORTS.h>
#include <aws/greengrass/GreengrassRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Greengrass
{
namespace Model
{

  /**
   */
  class AWS_GREENGRASS_API DeleteGroupRequest : public GreengrassRequest
  {
  public:
    DeleteGroupRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DeleteGroupRequest> Clone() const
    {
      return Aws::MakeUnique<DeleteGroupRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteGroup"; }

    Aws::String SerializePayload() const override;


    /**
     * The ID of the Greengrass group.
     */
    inline const Aws::String& GetGroupId() const{ return m_groupId; }

    /**
     * The ID of the Greengrass group.
     */
    inline bool GroupIdHasBeenSet() const { return m_groupIdHasBeenSet; }

    /**
     * The ID of the Greengrass group.
     */
    inline void SetGroupId(const Aws::String& value) { m_groupIdHasBeenSet = true; m_groupId = value; }

    /**
     * The ID of the Greengrass group.
     */
    inline void SetGroupId(Aws::String&& value) { m_groupIdHasBeenSet = true; m_groupId = std::move(value); }

    /**
     * The ID of the Greengrass group.
     */
    inline void SetGroupId(const char* value) { m_groupIdHasBeenSet = true; m_groupId.assign(value); }

    /**
     * The ID of the Greengrass group.
     */
    inline DeleteGroupRequest& WithGroupId(const Aws::String& value) { SetGroupId(value); return *this;}

    /**
     * The ID of the Greengrass group.
     */
    inline DeleteGroupRequest& WithGroupId(Aws::String&& value) { SetGroupId(std::move(value)); return *this;}

    /**
     * The ID of the Greengrass group.
     */
    inline DeleteGroupRequest& WithGroupId(const char* value) { SetGroupId(value); return *this;}

  private:

    Aws::String m_groupId;
    bool m_groupIdHasBeenSet = false;
  };

} // namespace Model
} // namespace Greengrass
} // namespace Aws
