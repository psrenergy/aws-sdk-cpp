﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/iot/IoT_EXPORTS.h>
#include <aws/iot/IoTRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace IoT
{
namespace Model
{

  /**
   */
  class AWS_IOT_API DescribeDetectMitigationActionsTaskRequest : public IoTRequest
  {
  public:
    DescribeDetectMitigationActionsTaskRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DescribeDetectMitigationActionsTaskRequest> Clone() const
    {
      return Aws::MakeUnique<DescribeDetectMitigationActionsTaskRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DescribeDetectMitigationActionsTask"; }

    Aws::String SerializePayload() const override;


    /**
     * <p> The unique identifier of the task. </p>
     */
    inline const Aws::String& GetTaskId() const{ return m_taskId; }

    /**
     * <p> The unique identifier of the task. </p>
     */
    inline bool TaskIdHasBeenSet() const { return m_taskIdHasBeenSet; }

    /**
     * <p> The unique identifier of the task. </p>
     */
    inline void SetTaskId(const Aws::String& value) { m_taskIdHasBeenSet = true; m_taskId = value; }

    /**
     * <p> The unique identifier of the task. </p>
     */
    inline void SetTaskId(Aws::String&& value) { m_taskIdHasBeenSet = true; m_taskId = std::move(value); }

    /**
     * <p> The unique identifier of the task. </p>
     */
    inline void SetTaskId(const char* value) { m_taskIdHasBeenSet = true; m_taskId.assign(value); }

    /**
     * <p> The unique identifier of the task. </p>
     */
    inline DescribeDetectMitigationActionsTaskRequest& WithTaskId(const Aws::String& value) { SetTaskId(value); return *this;}

    /**
     * <p> The unique identifier of the task. </p>
     */
    inline DescribeDetectMitigationActionsTaskRequest& WithTaskId(Aws::String&& value) { SetTaskId(std::move(value)); return *this;}

    /**
     * <p> The unique identifier of the task. </p>
     */
    inline DescribeDetectMitigationActionsTaskRequest& WithTaskId(const char* value) { SetTaskId(value); return *this;}

  private:

    Aws::String m_taskId;
    bool m_taskIdHasBeenSet = false;
  };

} // namespace Model
} // namespace IoT
} // namespace Aws
