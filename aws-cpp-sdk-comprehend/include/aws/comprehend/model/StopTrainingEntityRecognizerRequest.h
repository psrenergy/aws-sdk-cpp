﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/comprehend/Comprehend_EXPORTS.h>
#include <aws/comprehend/ComprehendRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Comprehend
{
namespace Model
{

  /**
   */
  class AWS_COMPREHEND_API StopTrainingEntityRecognizerRequest : public ComprehendRequest
  {
  public:
    StopTrainingEntityRecognizerRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<StopTrainingEntityRecognizerRequest> Clone() const
    {
      return Aws::MakeUnique<StopTrainingEntityRecognizerRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "StopTrainingEntityRecognizer"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The Amazon Resource Name (ARN) that identifies the entity recognizer
     * currently being trained.</p>
     */
    inline const Aws::String& GetEntityRecognizerArn() const{ return m_entityRecognizerArn; }

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the entity recognizer
     * currently being trained.</p>
     */
    inline bool EntityRecognizerArnHasBeenSet() const { return m_entityRecognizerArnHasBeenSet; }

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the entity recognizer
     * currently being trained.</p>
     */
    inline void SetEntityRecognizerArn(const Aws::String& value) { m_entityRecognizerArnHasBeenSet = true; m_entityRecognizerArn = value; }

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the entity recognizer
     * currently being trained.</p>
     */
    inline void SetEntityRecognizerArn(Aws::String&& value) { m_entityRecognizerArnHasBeenSet = true; m_entityRecognizerArn = std::move(value); }

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the entity recognizer
     * currently being trained.</p>
     */
    inline void SetEntityRecognizerArn(const char* value) { m_entityRecognizerArnHasBeenSet = true; m_entityRecognizerArn.assign(value); }

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the entity recognizer
     * currently being trained.</p>
     */
    inline StopTrainingEntityRecognizerRequest& WithEntityRecognizerArn(const Aws::String& value) { SetEntityRecognizerArn(value); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the entity recognizer
     * currently being trained.</p>
     */
    inline StopTrainingEntityRecognizerRequest& WithEntityRecognizerArn(Aws::String&& value) { SetEntityRecognizerArn(std::move(value)); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) that identifies the entity recognizer
     * currently being trained.</p>
     */
    inline StopTrainingEntityRecognizerRequest& WithEntityRecognizerArn(const char* value) { SetEntityRecognizerArn(value); return *this;}

  private:

    Aws::String m_entityRecognizerArn;
    bool m_entityRecognizerArnHasBeenSet = false;
  };

} // namespace Model
} // namespace Comprehend
} // namespace Aws
