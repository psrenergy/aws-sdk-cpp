﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/pinpoint/Pinpoint_EXPORTS.h>
#include <aws/pinpoint/PinpointRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Http
{
    class URI;
} //namespace Http
namespace Pinpoint
{
namespace Model
{

  /**
   */
  class AWS_PINPOINT_API DeleteEmailTemplateRequest : public PinpointRequest
  {
  public:
    DeleteEmailTemplateRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<DeleteEmailTemplateRequest> Clone() const
    {
      return Aws::MakeUnique<DeleteEmailTemplateRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteEmailTemplate"; }

    Aws::String SerializePayload() const override;

    void AddQueryStringParameters(Aws::Http::URI& uri) const override;


    /**
     * <p>The name of the message template. A template name must start with an
     * alphanumeric character and can contain a maximum of 128 characters. The
     * characters can be alphanumeric characters, underscores (_), or hyphens (-).
     * Template names are case sensitive.</p>
     */
    inline const Aws::String& GetTemplateName() const{ return m_templateName; }

    /**
     * <p>The name of the message template. A template name must start with an
     * alphanumeric character and can contain a maximum of 128 characters. The
     * characters can be alphanumeric characters, underscores (_), or hyphens (-).
     * Template names are case sensitive.</p>
     */
    inline bool TemplateNameHasBeenSet() const { return m_templateNameHasBeenSet; }

    /**
     * <p>The name of the message template. A template name must start with an
     * alphanumeric character and can contain a maximum of 128 characters. The
     * characters can be alphanumeric characters, underscores (_), or hyphens (-).
     * Template names are case sensitive.</p>
     */
    inline void SetTemplateName(const Aws::String& value) { m_templateNameHasBeenSet = true; m_templateName = value; }

    /**
     * <p>The name of the message template. A template name must start with an
     * alphanumeric character and can contain a maximum of 128 characters. The
     * characters can be alphanumeric characters, underscores (_), or hyphens (-).
     * Template names are case sensitive.</p>
     */
    inline void SetTemplateName(Aws::String&& value) { m_templateNameHasBeenSet = true; m_templateName = std::move(value); }

    /**
     * <p>The name of the message template. A template name must start with an
     * alphanumeric character and can contain a maximum of 128 characters. The
     * characters can be alphanumeric characters, underscores (_), or hyphens (-).
     * Template names are case sensitive.</p>
     */
    inline void SetTemplateName(const char* value) { m_templateNameHasBeenSet = true; m_templateName.assign(value); }

    /**
     * <p>The name of the message template. A template name must start with an
     * alphanumeric character and can contain a maximum of 128 characters. The
     * characters can be alphanumeric characters, underscores (_), or hyphens (-).
     * Template names are case sensitive.</p>
     */
    inline DeleteEmailTemplateRequest& WithTemplateName(const Aws::String& value) { SetTemplateName(value); return *this;}

    /**
     * <p>The name of the message template. A template name must start with an
     * alphanumeric character and can contain a maximum of 128 characters. The
     * characters can be alphanumeric characters, underscores (_), or hyphens (-).
     * Template names are case sensitive.</p>
     */
    inline DeleteEmailTemplateRequest& WithTemplateName(Aws::String&& value) { SetTemplateName(std::move(value)); return *this;}

    /**
     * <p>The name of the message template. A template name must start with an
     * alphanumeric character and can contain a maximum of 128 characters. The
     * characters can be alphanumeric characters, underscores (_), or hyphens (-).
     * Template names are case sensitive.</p>
     */
    inline DeleteEmailTemplateRequest& WithTemplateName(const char* value) { SetTemplateName(value); return *this;}


    /**
     * <p>The unique identifier for the version of the message template to update,
     * retrieve information about, or delete. To retrieve identifiers and other
     * information for all the versions of a template, use the <link 
     * linkend="templates-template-name-template-type-versions">Template
     * Versions</link> resource.</p> <p>If specified, this value must match the
     * identifier for an existing template version. If specified for an update
     * operation, this value must match the identifier for the latest existing version
     * of the template. This restriction helps ensure that race conditions don't
     * occur.</p> <p>If you don't specify a value for this parameter, Amazon Pinpoint
     * does the following:</p> <ul><li><p>For a get operation, retrieves information
     * about the active version of the template.</p></li> <li><p>For an update
     * operation, saves the updates to (overwrites) the latest existing version of the
     * template, if the create-new-version parameter isn't used or is set to
     * false.</p></li> <li><p>For a delete operation, deletes the template, including
     * all versions of the template.</p></li></ul>
     */
    inline const Aws::String& GetVersion() const{ return m_version; }

    /**
     * <p>The unique identifier for the version of the message template to update,
     * retrieve information about, or delete. To retrieve identifiers and other
     * information for all the versions of a template, use the <link 
     * linkend="templates-template-name-template-type-versions">Template
     * Versions</link> resource.</p> <p>If specified, this value must match the
     * identifier for an existing template version. If specified for an update
     * operation, this value must match the identifier for the latest existing version
     * of the template. This restriction helps ensure that race conditions don't
     * occur.</p> <p>If you don't specify a value for this parameter, Amazon Pinpoint
     * does the following:</p> <ul><li><p>For a get operation, retrieves information
     * about the active version of the template.</p></li> <li><p>For an update
     * operation, saves the updates to (overwrites) the latest existing version of the
     * template, if the create-new-version parameter isn't used or is set to
     * false.</p></li> <li><p>For a delete operation, deletes the template, including
     * all versions of the template.</p></li></ul>
     */
    inline bool VersionHasBeenSet() const { return m_versionHasBeenSet; }

    /**
     * <p>The unique identifier for the version of the message template to update,
     * retrieve information about, or delete. To retrieve identifiers and other
     * information for all the versions of a template, use the <link 
     * linkend="templates-template-name-template-type-versions">Template
     * Versions</link> resource.</p> <p>If specified, this value must match the
     * identifier for an existing template version. If specified for an update
     * operation, this value must match the identifier for the latest existing version
     * of the template. This restriction helps ensure that race conditions don't
     * occur.</p> <p>If you don't specify a value for this parameter, Amazon Pinpoint
     * does the following:</p> <ul><li><p>For a get operation, retrieves information
     * about the active version of the template.</p></li> <li><p>For an update
     * operation, saves the updates to (overwrites) the latest existing version of the
     * template, if the create-new-version parameter isn't used or is set to
     * false.</p></li> <li><p>For a delete operation, deletes the template, including
     * all versions of the template.</p></li></ul>
     */
    inline void SetVersion(const Aws::String& value) { m_versionHasBeenSet = true; m_version = value; }

    /**
     * <p>The unique identifier for the version of the message template to update,
     * retrieve information about, or delete. To retrieve identifiers and other
     * information for all the versions of a template, use the <link 
     * linkend="templates-template-name-template-type-versions">Template
     * Versions</link> resource.</p> <p>If specified, this value must match the
     * identifier for an existing template version. If specified for an update
     * operation, this value must match the identifier for the latest existing version
     * of the template. This restriction helps ensure that race conditions don't
     * occur.</p> <p>If you don't specify a value for this parameter, Amazon Pinpoint
     * does the following:</p> <ul><li><p>For a get operation, retrieves information
     * about the active version of the template.</p></li> <li><p>For an update
     * operation, saves the updates to (overwrites) the latest existing version of the
     * template, if the create-new-version parameter isn't used or is set to
     * false.</p></li> <li><p>For a delete operation, deletes the template, including
     * all versions of the template.</p></li></ul>
     */
    inline void SetVersion(Aws::String&& value) { m_versionHasBeenSet = true; m_version = std::move(value); }

    /**
     * <p>The unique identifier for the version of the message template to update,
     * retrieve information about, or delete. To retrieve identifiers and other
     * information for all the versions of a template, use the <link 
     * linkend="templates-template-name-template-type-versions">Template
     * Versions</link> resource.</p> <p>If specified, this value must match the
     * identifier for an existing template version. If specified for an update
     * operation, this value must match the identifier for the latest existing version
     * of the template. This restriction helps ensure that race conditions don't
     * occur.</p> <p>If you don't specify a value for this parameter, Amazon Pinpoint
     * does the following:</p> <ul><li><p>For a get operation, retrieves information
     * about the active version of the template.</p></li> <li><p>For an update
     * operation, saves the updates to (overwrites) the latest existing version of the
     * template, if the create-new-version parameter isn't used or is set to
     * false.</p></li> <li><p>For a delete operation, deletes the template, including
     * all versions of the template.</p></li></ul>
     */
    inline void SetVersion(const char* value) { m_versionHasBeenSet = true; m_version.assign(value); }

    /**
     * <p>The unique identifier for the version of the message template to update,
     * retrieve information about, or delete. To retrieve identifiers and other
     * information for all the versions of a template, use the <link 
     * linkend="templates-template-name-template-type-versions">Template
     * Versions</link> resource.</p> <p>If specified, this value must match the
     * identifier for an existing template version. If specified for an update
     * operation, this value must match the identifier for the latest existing version
     * of the template. This restriction helps ensure that race conditions don't
     * occur.</p> <p>If you don't specify a value for this parameter, Amazon Pinpoint
     * does the following:</p> <ul><li><p>For a get operation, retrieves information
     * about the active version of the template.</p></li> <li><p>For an update
     * operation, saves the updates to (overwrites) the latest existing version of the
     * template, if the create-new-version parameter isn't used or is set to
     * false.</p></li> <li><p>For a delete operation, deletes the template, including
     * all versions of the template.</p></li></ul>
     */
    inline DeleteEmailTemplateRequest& WithVersion(const Aws::String& value) { SetVersion(value); return *this;}

    /**
     * <p>The unique identifier for the version of the message template to update,
     * retrieve information about, or delete. To retrieve identifiers and other
     * information for all the versions of a template, use the <link 
     * linkend="templates-template-name-template-type-versions">Template
     * Versions</link> resource.</p> <p>If specified, this value must match the
     * identifier for an existing template version. If specified for an update
     * operation, this value must match the identifier for the latest existing version
     * of the template. This restriction helps ensure that race conditions don't
     * occur.</p> <p>If you don't specify a value for this parameter, Amazon Pinpoint
     * does the following:</p> <ul><li><p>For a get operation, retrieves information
     * about the active version of the template.</p></li> <li><p>For an update
     * operation, saves the updates to (overwrites) the latest existing version of the
     * template, if the create-new-version parameter isn't used or is set to
     * false.</p></li> <li><p>For a delete operation, deletes the template, including
     * all versions of the template.</p></li></ul>
     */
    inline DeleteEmailTemplateRequest& WithVersion(Aws::String&& value) { SetVersion(std::move(value)); return *this;}

    /**
     * <p>The unique identifier for the version of the message template to update,
     * retrieve information about, or delete. To retrieve identifiers and other
     * information for all the versions of a template, use the <link 
     * linkend="templates-template-name-template-type-versions">Template
     * Versions</link> resource.</p> <p>If specified, this value must match the
     * identifier for an existing template version. If specified for an update
     * operation, this value must match the identifier for the latest existing version
     * of the template. This restriction helps ensure that race conditions don't
     * occur.</p> <p>If you don't specify a value for this parameter, Amazon Pinpoint
     * does the following:</p> <ul><li><p>For a get operation, retrieves information
     * about the active version of the template.</p></li> <li><p>For an update
     * operation, saves the updates to (overwrites) the latest existing version of the
     * template, if the create-new-version parameter isn't used or is set to
     * false.</p></li> <li><p>For a delete operation, deletes the template, including
     * all versions of the template.</p></li></ul>
     */
    inline DeleteEmailTemplateRequest& WithVersion(const char* value) { SetVersion(value); return *this;}

  private:

    Aws::String m_templateName;
    bool m_templateNameHasBeenSet = false;

    Aws::String m_version;
    bool m_versionHasBeenSet = false;
  };

} // namespace Model
} // namespace Pinpoint
} // namespace Aws
