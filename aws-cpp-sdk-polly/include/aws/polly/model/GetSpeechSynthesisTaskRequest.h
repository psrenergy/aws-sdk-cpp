﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/polly/Polly_EXPORTS.h>
#include <aws/polly/PollyRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Polly
{
namespace Model
{

  /**
   */
  class AWS_POLLY_API GetSpeechSynthesisTaskRequest : public PollyRequest
  {
  public:
    GetSpeechSynthesisTaskRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<GetSpeechSynthesisTaskRequest> Clone() const
    {
      return Aws::MakeUnique<GetSpeechSynthesisTaskRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetSpeechSynthesisTask"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The Amazon Polly generated identifier for a speech synthesis task.</p>
     */
    inline const Aws::String& GetTaskId() const{ return m_taskId; }

    /**
     * <p>The Amazon Polly generated identifier for a speech synthesis task.</p>
     */
    inline bool TaskIdHasBeenSet() const { return m_taskIdHasBeenSet; }

    /**
     * <p>The Amazon Polly generated identifier for a speech synthesis task.</p>
     */
    inline void SetTaskId(const Aws::String& value) { m_taskIdHasBeenSet = true; m_taskId = value; }

    /**
     * <p>The Amazon Polly generated identifier for a speech synthesis task.</p>
     */
    inline void SetTaskId(Aws::String&& value) { m_taskIdHasBeenSet = true; m_taskId = std::move(value); }

    /**
     * <p>The Amazon Polly generated identifier for a speech synthesis task.</p>
     */
    inline void SetTaskId(const char* value) { m_taskIdHasBeenSet = true; m_taskId.assign(value); }

    /**
     * <p>The Amazon Polly generated identifier for a speech synthesis task.</p>
     */
    inline GetSpeechSynthesisTaskRequest& WithTaskId(const Aws::String& value) { SetTaskId(value); return *this;}

    /**
     * <p>The Amazon Polly generated identifier for a speech synthesis task.</p>
     */
    inline GetSpeechSynthesisTaskRequest& WithTaskId(Aws::String&& value) { SetTaskId(std::move(value)); return *this;}

    /**
     * <p>The Amazon Polly generated identifier for a speech synthesis task.</p>
     */
    inline GetSpeechSynthesisTaskRequest& WithTaskId(const char* value) { SetTaskId(value); return *this;}

  private:

    Aws::String m_taskId;
    bool m_taskIdHasBeenSet = false;
  };

} // namespace Model
} // namespace Polly
} // namespace Aws
