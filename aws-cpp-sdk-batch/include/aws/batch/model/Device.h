﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/batch/Batch_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/batch/model/DeviceCgroupPermission.h>
#include <utility>

namespace Aws
{
namespace Utils
{
namespace Json
{
  class JsonValue;
  class JsonView;
} // namespace Json
} // namespace Utils
namespace Batch
{
namespace Model
{

  /**
   * <p>An object that represents a container instance host device.</p> 
   * <p>This object isn't applicable to jobs that are running on Fargate resources
   * and shouldn't be provided.</p> <p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/batch-2016-08-10/Device">AWS API
   * Reference</a></p>
   */
  class AWS_BATCH_API Device
  {
  public:
    Device();
    Device(Aws::Utils::Json::JsonView jsonValue);
    Device& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The path for the device on the host container instance.</p>
     */
    inline const Aws::String& GetHostPath() const{ return m_hostPath; }

    /**
     * <p>The path for the device on the host container instance.</p>
     */
    inline bool HostPathHasBeenSet() const { return m_hostPathHasBeenSet; }

    /**
     * <p>The path for the device on the host container instance.</p>
     */
    inline void SetHostPath(const Aws::String& value) { m_hostPathHasBeenSet = true; m_hostPath = value; }

    /**
     * <p>The path for the device on the host container instance.</p>
     */
    inline void SetHostPath(Aws::String&& value) { m_hostPathHasBeenSet = true; m_hostPath = std::move(value); }

    /**
     * <p>The path for the device on the host container instance.</p>
     */
    inline void SetHostPath(const char* value) { m_hostPathHasBeenSet = true; m_hostPath.assign(value); }

    /**
     * <p>The path for the device on the host container instance.</p>
     */
    inline Device& WithHostPath(const Aws::String& value) { SetHostPath(value); return *this;}

    /**
     * <p>The path for the device on the host container instance.</p>
     */
    inline Device& WithHostPath(Aws::String&& value) { SetHostPath(std::move(value)); return *this;}

    /**
     * <p>The path for the device on the host container instance.</p>
     */
    inline Device& WithHostPath(const char* value) { SetHostPath(value); return *this;}


    /**
     * <p>The path inside the container that's used to expose the host device. By
     * default, the <code>hostPath</code> value is used.</p>
     */
    inline const Aws::String& GetContainerPath() const{ return m_containerPath; }

    /**
     * <p>The path inside the container that's used to expose the host device. By
     * default, the <code>hostPath</code> value is used.</p>
     */
    inline bool ContainerPathHasBeenSet() const { return m_containerPathHasBeenSet; }

    /**
     * <p>The path inside the container that's used to expose the host device. By
     * default, the <code>hostPath</code> value is used.</p>
     */
    inline void SetContainerPath(const Aws::String& value) { m_containerPathHasBeenSet = true; m_containerPath = value; }

    /**
     * <p>The path inside the container that's used to expose the host device. By
     * default, the <code>hostPath</code> value is used.</p>
     */
    inline void SetContainerPath(Aws::String&& value) { m_containerPathHasBeenSet = true; m_containerPath = std::move(value); }

    /**
     * <p>The path inside the container that's used to expose the host device. By
     * default, the <code>hostPath</code> value is used.</p>
     */
    inline void SetContainerPath(const char* value) { m_containerPathHasBeenSet = true; m_containerPath.assign(value); }

    /**
     * <p>The path inside the container that's used to expose the host device. By
     * default, the <code>hostPath</code> value is used.</p>
     */
    inline Device& WithContainerPath(const Aws::String& value) { SetContainerPath(value); return *this;}

    /**
     * <p>The path inside the container that's used to expose the host device. By
     * default, the <code>hostPath</code> value is used.</p>
     */
    inline Device& WithContainerPath(Aws::String&& value) { SetContainerPath(std::move(value)); return *this;}

    /**
     * <p>The path inside the container that's used to expose the host device. By
     * default, the <code>hostPath</code> value is used.</p>
     */
    inline Device& WithContainerPath(const char* value) { SetContainerPath(value); return *this;}


    /**
     * <p>The explicit permissions to provide to the container for the device. By
     * default, the container has permissions for <code>read</code>,
     * <code>write</code>, and <code>mknod</code> for the device.</p>
     */
    inline const Aws::Vector<DeviceCgroupPermission>& GetPermissions() const{ return m_permissions; }

    /**
     * <p>The explicit permissions to provide to the container for the device. By
     * default, the container has permissions for <code>read</code>,
     * <code>write</code>, and <code>mknod</code> for the device.</p>
     */
    inline bool PermissionsHasBeenSet() const { return m_permissionsHasBeenSet; }

    /**
     * <p>The explicit permissions to provide to the container for the device. By
     * default, the container has permissions for <code>read</code>,
     * <code>write</code>, and <code>mknod</code> for the device.</p>
     */
    inline void SetPermissions(const Aws::Vector<DeviceCgroupPermission>& value) { m_permissionsHasBeenSet = true; m_permissions = value; }

    /**
     * <p>The explicit permissions to provide to the container for the device. By
     * default, the container has permissions for <code>read</code>,
     * <code>write</code>, and <code>mknod</code> for the device.</p>
     */
    inline void SetPermissions(Aws::Vector<DeviceCgroupPermission>&& value) { m_permissionsHasBeenSet = true; m_permissions = std::move(value); }

    /**
     * <p>The explicit permissions to provide to the container for the device. By
     * default, the container has permissions for <code>read</code>,
     * <code>write</code>, and <code>mknod</code> for the device.</p>
     */
    inline Device& WithPermissions(const Aws::Vector<DeviceCgroupPermission>& value) { SetPermissions(value); return *this;}

    /**
     * <p>The explicit permissions to provide to the container for the device. By
     * default, the container has permissions for <code>read</code>,
     * <code>write</code>, and <code>mknod</code> for the device.</p>
     */
    inline Device& WithPermissions(Aws::Vector<DeviceCgroupPermission>&& value) { SetPermissions(std::move(value)); return *this;}

    /**
     * <p>The explicit permissions to provide to the container for the device. By
     * default, the container has permissions for <code>read</code>,
     * <code>write</code>, and <code>mknod</code> for the device.</p>
     */
    inline Device& AddPermissions(const DeviceCgroupPermission& value) { m_permissionsHasBeenSet = true; m_permissions.push_back(value); return *this; }

    /**
     * <p>The explicit permissions to provide to the container for the device. By
     * default, the container has permissions for <code>read</code>,
     * <code>write</code>, and <code>mknod</code> for the device.</p>
     */
    inline Device& AddPermissions(DeviceCgroupPermission&& value) { m_permissionsHasBeenSet = true; m_permissions.push_back(std::move(value)); return *this; }

  private:

    Aws::String m_hostPath;
    bool m_hostPathHasBeenSet = false;

    Aws::String m_containerPath;
    bool m_containerPathHasBeenSet = false;

    Aws::Vector<DeviceCgroupPermission> m_permissions;
    bool m_permissionsHasBeenSet = false;
  };

} // namespace Model
} // namespace Batch
} // namespace Aws
