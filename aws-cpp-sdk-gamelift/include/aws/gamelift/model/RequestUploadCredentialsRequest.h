﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/gamelift/GameLift_EXPORTS.h>
#include <aws/gamelift/GameLiftRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace GameLift
{
namespace Model
{

  /**
   */
  class AWS_GAMELIFT_API RequestUploadCredentialsRequest : public GameLiftRequest
  {
  public:
    RequestUploadCredentialsRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "RequestUploadCredentials"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>A unique identifier for the build to get credentials for. You can use either
     * the build ID or ARN value. </p>
     */
    inline const Aws::String& GetBuildId() const{ return m_buildId; }

    /**
     * <p>A unique identifier for the build to get credentials for. You can use either
     * the build ID or ARN value. </p>
     */
    inline bool BuildIdHasBeenSet() const { return m_buildIdHasBeenSet; }

    /**
     * <p>A unique identifier for the build to get credentials for. You can use either
     * the build ID or ARN value. </p>
     */
    inline void SetBuildId(const Aws::String& value) { m_buildIdHasBeenSet = true; m_buildId = value; }

    /**
     * <p>A unique identifier for the build to get credentials for. You can use either
     * the build ID or ARN value. </p>
     */
    inline void SetBuildId(Aws::String&& value) { m_buildIdHasBeenSet = true; m_buildId = std::move(value); }

    /**
     * <p>A unique identifier for the build to get credentials for. You can use either
     * the build ID or ARN value. </p>
     */
    inline void SetBuildId(const char* value) { m_buildIdHasBeenSet = true; m_buildId.assign(value); }

    /**
     * <p>A unique identifier for the build to get credentials for. You can use either
     * the build ID or ARN value. </p>
     */
    inline RequestUploadCredentialsRequest& WithBuildId(const Aws::String& value) { SetBuildId(value); return *this;}

    /**
     * <p>A unique identifier for the build to get credentials for. You can use either
     * the build ID or ARN value. </p>
     */
    inline RequestUploadCredentialsRequest& WithBuildId(Aws::String&& value) { SetBuildId(std::move(value)); return *this;}

    /**
     * <p>A unique identifier for the build to get credentials for. You can use either
     * the build ID or ARN value. </p>
     */
    inline RequestUploadCredentialsRequest& WithBuildId(const char* value) { SetBuildId(value); return *this;}

  private:

    Aws::String m_buildId;
    bool m_buildIdHasBeenSet = false;
  };

} // namespace Model
} // namespace GameLift
} // namespace Aws
