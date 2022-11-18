﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/backup/Backup_EXPORTS.h>
#include <aws/backup/BackupRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Backup
{
namespace Model
{

  /**
   */
  class AWS_BACKUP_API DeleteFrameworkRequest : public BackupRequest
  {
  public:
    DeleteFrameworkRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DeleteFrameworkRequest> Clone() const
    {
      return Aws::MakeUnique<DeleteFrameworkRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteFramework"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The unique name of a framework.</p>
     */
    inline const Aws::String& GetFrameworkName() const{ return m_frameworkName; }

    /**
     * <p>The unique name of a framework.</p>
     */
    inline bool FrameworkNameHasBeenSet() const { return m_frameworkNameHasBeenSet; }

    /**
     * <p>The unique name of a framework.</p>
     */
    inline void SetFrameworkName(const Aws::String& value) { m_frameworkNameHasBeenSet = true; m_frameworkName = value; }

    /**
     * <p>The unique name of a framework.</p>
     */
    inline void SetFrameworkName(Aws::String&& value) { m_frameworkNameHasBeenSet = true; m_frameworkName = std::move(value); }

    /**
     * <p>The unique name of a framework.</p>
     */
    inline void SetFrameworkName(const char* value) { m_frameworkNameHasBeenSet = true; m_frameworkName.assign(value); }

    /**
     * <p>The unique name of a framework.</p>
     */
    inline DeleteFrameworkRequest& WithFrameworkName(const Aws::String& value) { SetFrameworkName(value); return *this;}

    /**
     * <p>The unique name of a framework.</p>
     */
    inline DeleteFrameworkRequest& WithFrameworkName(Aws::String&& value) { SetFrameworkName(std::move(value)); return *this;}

    /**
     * <p>The unique name of a framework.</p>
     */
    inline DeleteFrameworkRequest& WithFrameworkName(const char* value) { SetFrameworkName(value); return *this;}

  private:

    Aws::String m_frameworkName;
    bool m_frameworkNameHasBeenSet = false;
  };

} // namespace Model
} // namespace Backup
} // namespace Aws
