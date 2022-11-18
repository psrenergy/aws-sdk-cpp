﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/codedeploy/CodeDeploy_EXPORTS.h>
#include <aws/codedeploy/CodeDeployRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace CodeDeploy
{
namespace Model
{

  /**
   * <p>Represents the input of a <code>GetDeploymentConfig</code>
   * operation.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/codedeploy-2014-10-06/GetDeploymentConfigInput">AWS
   * API Reference</a></p>
   */
  class AWS_CODEDEPLOY_API GetDeploymentConfigRequest : public CodeDeployRequest
  {
  public:
    GetDeploymentConfigRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetDeploymentConfigRequest> Clone() const
    {
      return Aws::MakeUnique<GetDeploymentConfigRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetDeploymentConfig"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The name of a deployment configuration associated with the IAM user or Amazon
     * Web Services account.</p>
     */
    inline const Aws::String& GetDeploymentConfigName() const{ return m_deploymentConfigName; }

    /**
     * <p>The name of a deployment configuration associated with the IAM user or Amazon
     * Web Services account.</p>
     */
    inline bool DeploymentConfigNameHasBeenSet() const { return m_deploymentConfigNameHasBeenSet; }

    /**
     * <p>The name of a deployment configuration associated with the IAM user or Amazon
     * Web Services account.</p>
     */
    inline void SetDeploymentConfigName(const Aws::String& value) { m_deploymentConfigNameHasBeenSet = true; m_deploymentConfigName = value; }

    /**
     * <p>The name of a deployment configuration associated with the IAM user or Amazon
     * Web Services account.</p>
     */
    inline void SetDeploymentConfigName(Aws::String&& value) { m_deploymentConfigNameHasBeenSet = true; m_deploymentConfigName = std::move(value); }

    /**
     * <p>The name of a deployment configuration associated with the IAM user or Amazon
     * Web Services account.</p>
     */
    inline void SetDeploymentConfigName(const char* value) { m_deploymentConfigNameHasBeenSet = true; m_deploymentConfigName.assign(value); }

    /**
     * <p>The name of a deployment configuration associated with the IAM user or Amazon
     * Web Services account.</p>
     */
    inline GetDeploymentConfigRequest& WithDeploymentConfigName(const Aws::String& value) { SetDeploymentConfigName(value); return *this;}

    /**
     * <p>The name of a deployment configuration associated with the IAM user or Amazon
     * Web Services account.</p>
     */
    inline GetDeploymentConfigRequest& WithDeploymentConfigName(Aws::String&& value) { SetDeploymentConfigName(std::move(value)); return *this;}

    /**
     * <p>The name of a deployment configuration associated with the IAM user or Amazon
     * Web Services account.</p>
     */
    inline GetDeploymentConfigRequest& WithDeploymentConfigName(const char* value) { SetDeploymentConfigName(value); return *this;}

  private:

    Aws::String m_deploymentConfigName;
    bool m_deploymentConfigNameHasBeenSet = false;
  };

} // namespace Model
} // namespace CodeDeploy
} // namespace Aws
