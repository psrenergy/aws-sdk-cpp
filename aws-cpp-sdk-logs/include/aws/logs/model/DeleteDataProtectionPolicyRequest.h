﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/logs/CloudWatchLogs_EXPORTS.h>
#include <aws/logs/CloudWatchLogsRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace CloudWatchLogs
{
namespace Model
{

  /**
   */
  class AWS_CLOUDWATCHLOGS_API DeleteDataProtectionPolicyRequest : public CloudWatchLogsRequest
  {
  public:
    DeleteDataProtectionPolicyRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteDataProtectionPolicy"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The name or ARN of the log group that you want to delete the data protection
     * policy for.</p>
     */
    inline const Aws::String& GetLogGroupIdentifier() const{ return m_logGroupIdentifier; }

    /**
     * <p>The name or ARN of the log group that you want to delete the data protection
     * policy for.</p>
     */
    inline bool LogGroupIdentifierHasBeenSet() const { return m_logGroupIdentifierHasBeenSet; }

    /**
     * <p>The name or ARN of the log group that you want to delete the data protection
     * policy for.</p>
     */
    inline void SetLogGroupIdentifier(const Aws::String& value) { m_logGroupIdentifierHasBeenSet = true; m_logGroupIdentifier = value; }

    /**
     * <p>The name or ARN of the log group that you want to delete the data protection
     * policy for.</p>
     */
    inline void SetLogGroupIdentifier(Aws::String&& value) { m_logGroupIdentifierHasBeenSet = true; m_logGroupIdentifier = std::move(value); }

    /**
     * <p>The name or ARN of the log group that you want to delete the data protection
     * policy for.</p>
     */
    inline void SetLogGroupIdentifier(const char* value) { m_logGroupIdentifierHasBeenSet = true; m_logGroupIdentifier.assign(value); }

    /**
     * <p>The name or ARN of the log group that you want to delete the data protection
     * policy for.</p>
     */
    inline DeleteDataProtectionPolicyRequest& WithLogGroupIdentifier(const Aws::String& value) { SetLogGroupIdentifier(value); return *this;}

    /**
     * <p>The name or ARN of the log group that you want to delete the data protection
     * policy for.</p>
     */
    inline DeleteDataProtectionPolicyRequest& WithLogGroupIdentifier(Aws::String&& value) { SetLogGroupIdentifier(std::move(value)); return *this;}

    /**
     * <p>The name or ARN of the log group that you want to delete the data protection
     * policy for.</p>
     */
    inline DeleteDataProtectionPolicyRequest& WithLogGroupIdentifier(const char* value) { SetLogGroupIdentifier(value); return *this;}

  private:

    Aws::String m_logGroupIdentifier;
    bool m_logGroupIdentifierHasBeenSet = false;
  };

} // namespace Model
} // namespace CloudWatchLogs
} // namespace Aws
