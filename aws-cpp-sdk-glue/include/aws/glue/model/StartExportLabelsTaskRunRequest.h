﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/glue/Glue_EXPORTS.h>
#include <aws/glue/GlueRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Glue
{
namespace Model
{

  /**
   */
  class AWS_GLUE_API StartExportLabelsTaskRunRequest : public GlueRequest
  {
  public:
    StartExportLabelsTaskRunRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<StartExportLabelsTaskRunRequest> Clone() const
    {
      return Aws::MakeUnique<StartExportLabelsTaskRunRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "StartExportLabelsTaskRun"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The unique identifier of the machine learning transform.</p>
     */
    inline const Aws::String& GetTransformId() const{ return m_transformId; }

    /**
     * <p>The unique identifier of the machine learning transform.</p>
     */
    inline bool TransformIdHasBeenSet() const { return m_transformIdHasBeenSet; }

    /**
     * <p>The unique identifier of the machine learning transform.</p>
     */
    inline void SetTransformId(const Aws::String& value) { m_transformIdHasBeenSet = true; m_transformId = value; }

    /**
     * <p>The unique identifier of the machine learning transform.</p>
     */
    inline void SetTransformId(Aws::String&& value) { m_transformIdHasBeenSet = true; m_transformId = std::move(value); }

    /**
     * <p>The unique identifier of the machine learning transform.</p>
     */
    inline void SetTransformId(const char* value) { m_transformIdHasBeenSet = true; m_transformId.assign(value); }

    /**
     * <p>The unique identifier of the machine learning transform.</p>
     */
    inline StartExportLabelsTaskRunRequest& WithTransformId(const Aws::String& value) { SetTransformId(value); return *this;}

    /**
     * <p>The unique identifier of the machine learning transform.</p>
     */
    inline StartExportLabelsTaskRunRequest& WithTransformId(Aws::String&& value) { SetTransformId(std::move(value)); return *this;}

    /**
     * <p>The unique identifier of the machine learning transform.</p>
     */
    inline StartExportLabelsTaskRunRequest& WithTransformId(const char* value) { SetTransformId(value); return *this;}


    /**
     * <p>The Amazon S3 path where you export the labels.</p>
     */
    inline const Aws::String& GetOutputS3Path() const{ return m_outputS3Path; }

    /**
     * <p>The Amazon S3 path where you export the labels.</p>
     */
    inline bool OutputS3PathHasBeenSet() const { return m_outputS3PathHasBeenSet; }

    /**
     * <p>The Amazon S3 path where you export the labels.</p>
     */
    inline void SetOutputS3Path(const Aws::String& value) { m_outputS3PathHasBeenSet = true; m_outputS3Path = value; }

    /**
     * <p>The Amazon S3 path where you export the labels.</p>
     */
    inline void SetOutputS3Path(Aws::String&& value) { m_outputS3PathHasBeenSet = true; m_outputS3Path = std::move(value); }

    /**
     * <p>The Amazon S3 path where you export the labels.</p>
     */
    inline void SetOutputS3Path(const char* value) { m_outputS3PathHasBeenSet = true; m_outputS3Path.assign(value); }

    /**
     * <p>The Amazon S3 path where you export the labels.</p>
     */
    inline StartExportLabelsTaskRunRequest& WithOutputS3Path(const Aws::String& value) { SetOutputS3Path(value); return *this;}

    /**
     * <p>The Amazon S3 path where you export the labels.</p>
     */
    inline StartExportLabelsTaskRunRequest& WithOutputS3Path(Aws::String&& value) { SetOutputS3Path(std::move(value)); return *this;}

    /**
     * <p>The Amazon S3 path where you export the labels.</p>
     */
    inline StartExportLabelsTaskRunRequest& WithOutputS3Path(const char* value) { SetOutputS3Path(value); return *this;}

  private:

    Aws::String m_transformId;
    bool m_transformIdHasBeenSet = false;

    Aws::String m_outputS3Path;
    bool m_outputS3PathHasBeenSet = false;
  };

} // namespace Model
} // namespace Glue
} // namespace Aws
