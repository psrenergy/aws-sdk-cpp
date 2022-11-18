﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/migrationhuborchestrator/MigrationHubOrchestrator_EXPORTS.h>
#include <aws/migrationhuborchestrator/MigrationHubOrchestratorRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Http
{
    class URI;
} //namespace Http
namespace MigrationHubOrchestrator
{
namespace Model
{

  /**
   */
  class AWS_MIGRATIONHUBORCHESTRATOR_API GetWorkflowStepRequest : public MigrationHubOrchestratorRequest
  {
  public:
    GetWorkflowStepRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetWorkflowStepRequest> Clone() const
    {
      return Aws::MakeUnique<GetWorkflowStepRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetWorkflowStep"; }

    Aws::String SerializePayload() const override;

    void AddQueryStringParameters(Aws::Http::URI& uri) const override;


    /**
     * <p>The ID of the migration workflow.</p>
     */
    inline const Aws::String& GetWorkflowId() const{ return m_workflowId; }

    /**
     * <p>The ID of the migration workflow.</p>
     */
    inline bool WorkflowIdHasBeenSet() const { return m_workflowIdHasBeenSet; }

    /**
     * <p>The ID of the migration workflow.</p>
     */
    inline void SetWorkflowId(const Aws::String& value) { m_workflowIdHasBeenSet = true; m_workflowId = value; }

    /**
     * <p>The ID of the migration workflow.</p>
     */
    inline void SetWorkflowId(Aws::String&& value) { m_workflowIdHasBeenSet = true; m_workflowId = std::move(value); }

    /**
     * <p>The ID of the migration workflow.</p>
     */
    inline void SetWorkflowId(const char* value) { m_workflowIdHasBeenSet = true; m_workflowId.assign(value); }

    /**
     * <p>The ID of the migration workflow.</p>
     */
    inline GetWorkflowStepRequest& WithWorkflowId(const Aws::String& value) { SetWorkflowId(value); return *this;}

    /**
     * <p>The ID of the migration workflow.</p>
     */
    inline GetWorkflowStepRequest& WithWorkflowId(Aws::String&& value) { SetWorkflowId(std::move(value)); return *this;}

    /**
     * <p>The ID of the migration workflow.</p>
     */
    inline GetWorkflowStepRequest& WithWorkflowId(const char* value) { SetWorkflowId(value); return *this;}


    /**
     * <p>desThe ID of the step group.</p>
     */
    inline const Aws::String& GetStepGroupId() const{ return m_stepGroupId; }

    /**
     * <p>desThe ID of the step group.</p>
     */
    inline bool StepGroupIdHasBeenSet() const { return m_stepGroupIdHasBeenSet; }

    /**
     * <p>desThe ID of the step group.</p>
     */
    inline void SetStepGroupId(const Aws::String& value) { m_stepGroupIdHasBeenSet = true; m_stepGroupId = value; }

    /**
     * <p>desThe ID of the step group.</p>
     */
    inline void SetStepGroupId(Aws::String&& value) { m_stepGroupIdHasBeenSet = true; m_stepGroupId = std::move(value); }

    /**
     * <p>desThe ID of the step group.</p>
     */
    inline void SetStepGroupId(const char* value) { m_stepGroupIdHasBeenSet = true; m_stepGroupId.assign(value); }

    /**
     * <p>desThe ID of the step group.</p>
     */
    inline GetWorkflowStepRequest& WithStepGroupId(const Aws::String& value) { SetStepGroupId(value); return *this;}

    /**
     * <p>desThe ID of the step group.</p>
     */
    inline GetWorkflowStepRequest& WithStepGroupId(Aws::String&& value) { SetStepGroupId(std::move(value)); return *this;}

    /**
     * <p>desThe ID of the step group.</p>
     */
    inline GetWorkflowStepRequest& WithStepGroupId(const char* value) { SetStepGroupId(value); return *this;}


    /**
     * <p>The ID of the step.</p>
     */
    inline const Aws::String& GetId() const{ return m_id; }

    /**
     * <p>The ID of the step.</p>
     */
    inline bool IdHasBeenSet() const { return m_idHasBeenSet; }

    /**
     * <p>The ID of the step.</p>
     */
    inline void SetId(const Aws::String& value) { m_idHasBeenSet = true; m_id = value; }

    /**
     * <p>The ID of the step.</p>
     */
    inline void SetId(Aws::String&& value) { m_idHasBeenSet = true; m_id = std::move(value); }

    /**
     * <p>The ID of the step.</p>
     */
    inline void SetId(const char* value) { m_idHasBeenSet = true; m_id.assign(value); }

    /**
     * <p>The ID of the step.</p>
     */
    inline GetWorkflowStepRequest& WithId(const Aws::String& value) { SetId(value); return *this;}

    /**
     * <p>The ID of the step.</p>
     */
    inline GetWorkflowStepRequest& WithId(Aws::String&& value) { SetId(std::move(value)); return *this;}

    /**
     * <p>The ID of the step.</p>
     */
    inline GetWorkflowStepRequest& WithId(const char* value) { SetId(value); return *this;}

  private:

    Aws::String m_workflowId;
    bool m_workflowIdHasBeenSet = false;

    Aws::String m_stepGroupId;
    bool m_stepGroupIdHasBeenSet = false;

    Aws::String m_id;
    bool m_idHasBeenSet = false;
  };

} // namespace Model
} // namespace MigrationHubOrchestrator
} // namespace Aws
