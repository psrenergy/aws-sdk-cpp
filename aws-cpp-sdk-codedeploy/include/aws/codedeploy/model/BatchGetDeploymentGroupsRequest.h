﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/codedeploy/CodeDeploy_EXPORTS.h>
#include <aws/codedeploy/CodeDeployRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <utility>

namespace Aws
{
namespace CodeDeploy
{
namespace Model
{

  /**
   * <p>Represents the input of a <code>BatchGetDeploymentGroups</code>
   * operation.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/codedeploy-2014-10-06/BatchGetDeploymentGroupsInput">AWS
   * API Reference</a></p>
   */
  class AWS_CODEDEPLOY_API BatchGetDeploymentGroupsRequest : public CodeDeployRequest
  {
  public:
    BatchGetDeploymentGroupsRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<BatchGetDeploymentGroupsRequest> Clone() const
    {
      return Aws::MakeUnique<BatchGetDeploymentGroupsRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "BatchGetDeploymentGroups"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The name of an CodeDeploy application associated with the applicable IAM or
     * Amazon Web Services account.</p>
     */
    inline const Aws::String& GetApplicationName() const{ return m_applicationName; }

    /**
     * <p>The name of an CodeDeploy application associated with the applicable IAM or
     * Amazon Web Services account.</p>
     */
    inline bool ApplicationNameHasBeenSet() const { return m_applicationNameHasBeenSet; }

    /**
     * <p>The name of an CodeDeploy application associated with the applicable IAM or
     * Amazon Web Services account.</p>
     */
    inline void SetApplicationName(const Aws::String& value) { m_applicationNameHasBeenSet = true; m_applicationName = value; }

    /**
     * <p>The name of an CodeDeploy application associated with the applicable IAM or
     * Amazon Web Services account.</p>
     */
    inline void SetApplicationName(Aws::String&& value) { m_applicationNameHasBeenSet = true; m_applicationName = std::move(value); }

    /**
     * <p>The name of an CodeDeploy application associated with the applicable IAM or
     * Amazon Web Services account.</p>
     */
    inline void SetApplicationName(const char* value) { m_applicationNameHasBeenSet = true; m_applicationName.assign(value); }

    /**
     * <p>The name of an CodeDeploy application associated with the applicable IAM or
     * Amazon Web Services account.</p>
     */
    inline BatchGetDeploymentGroupsRequest& WithApplicationName(const Aws::String& value) { SetApplicationName(value); return *this;}

    /**
     * <p>The name of an CodeDeploy application associated with the applicable IAM or
     * Amazon Web Services account.</p>
     */
    inline BatchGetDeploymentGroupsRequest& WithApplicationName(Aws::String&& value) { SetApplicationName(std::move(value)); return *this;}

    /**
     * <p>The name of an CodeDeploy application associated with the applicable IAM or
     * Amazon Web Services account.</p>
     */
    inline BatchGetDeploymentGroupsRequest& WithApplicationName(const char* value) { SetApplicationName(value); return *this;}


    /**
     * <p>The names of the deployment groups.</p>
     */
    inline const Aws::Vector<Aws::String>& GetDeploymentGroupNames() const{ return m_deploymentGroupNames; }

    /**
     * <p>The names of the deployment groups.</p>
     */
    inline bool DeploymentGroupNamesHasBeenSet() const { return m_deploymentGroupNamesHasBeenSet; }

    /**
     * <p>The names of the deployment groups.</p>
     */
    inline void SetDeploymentGroupNames(const Aws::Vector<Aws::String>& value) { m_deploymentGroupNamesHasBeenSet = true; m_deploymentGroupNames = value; }

    /**
     * <p>The names of the deployment groups.</p>
     */
    inline void SetDeploymentGroupNames(Aws::Vector<Aws::String>&& value) { m_deploymentGroupNamesHasBeenSet = true; m_deploymentGroupNames = std::move(value); }

    /**
     * <p>The names of the deployment groups.</p>
     */
    inline BatchGetDeploymentGroupsRequest& WithDeploymentGroupNames(const Aws::Vector<Aws::String>& value) { SetDeploymentGroupNames(value); return *this;}

    /**
     * <p>The names of the deployment groups.</p>
     */
    inline BatchGetDeploymentGroupsRequest& WithDeploymentGroupNames(Aws::Vector<Aws::String>&& value) { SetDeploymentGroupNames(std::move(value)); return *this;}

    /**
     * <p>The names of the deployment groups.</p>
     */
    inline BatchGetDeploymentGroupsRequest& AddDeploymentGroupNames(const Aws::String& value) { m_deploymentGroupNamesHasBeenSet = true; m_deploymentGroupNames.push_back(value); return *this; }

    /**
     * <p>The names of the deployment groups.</p>
     */
    inline BatchGetDeploymentGroupsRequest& AddDeploymentGroupNames(Aws::String&& value) { m_deploymentGroupNamesHasBeenSet = true; m_deploymentGroupNames.push_back(std::move(value)); return *this; }

    /**
     * <p>The names of the deployment groups.</p>
     */
    inline BatchGetDeploymentGroupsRequest& AddDeploymentGroupNames(const char* value) { m_deploymentGroupNamesHasBeenSet = true; m_deploymentGroupNames.push_back(value); return *this; }

  private:

    Aws::String m_applicationName;
    bool m_applicationNameHasBeenSet = false;

    Aws::Vector<Aws::String> m_deploymentGroupNames;
    bool m_deploymentGroupNamesHasBeenSet = false;
  };

} // namespace Model
} // namespace CodeDeploy
} // namespace Aws
