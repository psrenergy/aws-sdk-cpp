﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/elasticfilesystem/EFS_EXPORTS.h>
#include <aws/elasticfilesystem/EFSRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace EFS
{
namespace Model
{

  /**
   */
  class AWS_EFS_API DeleteFileSystemPolicyRequest : public EFSRequest
  {
  public:
    DeleteFileSystemPolicyRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DeleteFileSystemPolicyRequest> Clone() const
    {
      return Aws::MakeUnique<DeleteFileSystemPolicyRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteFileSystemPolicy"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>Specifies the EFS file system for which to delete the
     * <code>FileSystemPolicy</code>.</p>
     */
    inline const Aws::String& GetFileSystemId() const{ return m_fileSystemId; }

    /**
     * <p>Specifies the EFS file system for which to delete the
     * <code>FileSystemPolicy</code>.</p>
     */
    inline bool FileSystemIdHasBeenSet() const { return m_fileSystemIdHasBeenSet; }

    /**
     * <p>Specifies the EFS file system for which to delete the
     * <code>FileSystemPolicy</code>.</p>
     */
    inline void SetFileSystemId(const Aws::String& value) { m_fileSystemIdHasBeenSet = true; m_fileSystemId = value; }

    /**
     * <p>Specifies the EFS file system for which to delete the
     * <code>FileSystemPolicy</code>.</p>
     */
    inline void SetFileSystemId(Aws::String&& value) { m_fileSystemIdHasBeenSet = true; m_fileSystemId = std::move(value); }

    /**
     * <p>Specifies the EFS file system for which to delete the
     * <code>FileSystemPolicy</code>.</p>
     */
    inline void SetFileSystemId(const char* value) { m_fileSystemIdHasBeenSet = true; m_fileSystemId.assign(value); }

    /**
     * <p>Specifies the EFS file system for which to delete the
     * <code>FileSystemPolicy</code>.</p>
     */
    inline DeleteFileSystemPolicyRequest& WithFileSystemId(const Aws::String& value) { SetFileSystemId(value); return *this;}

    /**
     * <p>Specifies the EFS file system for which to delete the
     * <code>FileSystemPolicy</code>.</p>
     */
    inline DeleteFileSystemPolicyRequest& WithFileSystemId(Aws::String&& value) { SetFileSystemId(std::move(value)); return *this;}

    /**
     * <p>Specifies the EFS file system for which to delete the
     * <code>FileSystemPolicy</code>.</p>
     */
    inline DeleteFileSystemPolicyRequest& WithFileSystemId(const char* value) { SetFileSystemId(value); return *this;}

  private:

    Aws::String m_fileSystemId;
    bool m_fileSystemIdHasBeenSet = false;
  };

} // namespace Model
} // namespace EFS
} // namespace Aws
