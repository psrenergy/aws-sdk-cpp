﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/codeguruprofiler/CodeGuruProfiler_EXPORTS.h>
#include <aws/codeguruprofiler/CodeGuruProfilerRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace CodeGuruProfiler
{
namespace Model
{

  /**
   * <p>The structure representing the describeProfilingGroupRequest.</p><p><h3>See
   * Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/codeguruprofiler-2019-07-18/DescribeProfilingGroupRequest">AWS
   * API Reference</a></p>
   */
  class AWS_CODEGURUPROFILER_API DescribeProfilingGroupRequest : public CodeGuruProfilerRequest
  {
  public:
    DescribeProfilingGroupRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DescribeProfilingGroupRequest> Clone() const
    {
      return Aws::MakeUnique<DescribeProfilingGroupRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DescribeProfilingGroup"; }

    Aws::String SerializePayload() const override;


    /**
     * <p> The name of the profiling group to get information about. </p>
     */
    inline const Aws::String& GetProfilingGroupName() const{ return m_profilingGroupName; }

    /**
     * <p> The name of the profiling group to get information about. </p>
     */
    inline bool ProfilingGroupNameHasBeenSet() const { return m_profilingGroupNameHasBeenSet; }

    /**
     * <p> The name of the profiling group to get information about. </p>
     */
    inline void SetProfilingGroupName(const Aws::String& value) { m_profilingGroupNameHasBeenSet = true; m_profilingGroupName = value; }

    /**
     * <p> The name of the profiling group to get information about. </p>
     */
    inline void SetProfilingGroupName(Aws::String&& value) { m_profilingGroupNameHasBeenSet = true; m_profilingGroupName = std::move(value); }

    /**
     * <p> The name of the profiling group to get information about. </p>
     */
    inline void SetProfilingGroupName(const char* value) { m_profilingGroupNameHasBeenSet = true; m_profilingGroupName.assign(value); }

    /**
     * <p> The name of the profiling group to get information about. </p>
     */
    inline DescribeProfilingGroupRequest& WithProfilingGroupName(const Aws::String& value) { SetProfilingGroupName(value); return *this;}

    /**
     * <p> The name of the profiling group to get information about. </p>
     */
    inline DescribeProfilingGroupRequest& WithProfilingGroupName(Aws::String&& value) { SetProfilingGroupName(std::move(value)); return *this;}

    /**
     * <p> The name of the profiling group to get information about. </p>
     */
    inline DescribeProfilingGroupRequest& WithProfilingGroupName(const char* value) { SetProfilingGroupName(value); return *this;}

  private:

    Aws::String m_profilingGroupName;
    bool m_profilingGroupNameHasBeenSet = false;
  };

} // namespace Model
} // namespace CodeGuruProfiler
} // namespace Aws
