﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/athena/Athena_EXPORTS.h>
#include <aws/athena/AthenaRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Athena
{
namespace Model
{

  /**
   */
  class AWS_ATHENA_API GetNotebookMetadataRequest : public AthenaRequest
  {
  public:
    GetNotebookMetadataRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "GetNotebookMetadata"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The ID of the notebook whose metadata is to be retrieved.</p>
     */
    inline const Aws::String& GetNotebookId() const{ return m_notebookId; }

    /**
     * <p>The ID of the notebook whose metadata is to be retrieved.</p>
     */
    inline bool NotebookIdHasBeenSet() const { return m_notebookIdHasBeenSet; }

    /**
     * <p>The ID of the notebook whose metadata is to be retrieved.</p>
     */
    inline void SetNotebookId(const Aws::String& value) { m_notebookIdHasBeenSet = true; m_notebookId = value; }

    /**
     * <p>The ID of the notebook whose metadata is to be retrieved.</p>
     */
    inline void SetNotebookId(Aws::String&& value) { m_notebookIdHasBeenSet = true; m_notebookId = std::move(value); }

    /**
     * <p>The ID of the notebook whose metadata is to be retrieved.</p>
     */
    inline void SetNotebookId(const char* value) { m_notebookIdHasBeenSet = true; m_notebookId.assign(value); }

    /**
     * <p>The ID of the notebook whose metadata is to be retrieved.</p>
     */
    inline GetNotebookMetadataRequest& WithNotebookId(const Aws::String& value) { SetNotebookId(value); return *this;}

    /**
     * <p>The ID of the notebook whose metadata is to be retrieved.</p>
     */
    inline GetNotebookMetadataRequest& WithNotebookId(Aws::String&& value) { SetNotebookId(std::move(value)); return *this;}

    /**
     * <p>The ID of the notebook whose metadata is to be retrieved.</p>
     */
    inline GetNotebookMetadataRequest& WithNotebookId(const char* value) { SetNotebookId(value); return *this;}

  private:

    Aws::String m_notebookId;
    bool m_notebookIdHasBeenSet = false;
  };

} // namespace Model
} // namespace Athena
} // namespace Aws
