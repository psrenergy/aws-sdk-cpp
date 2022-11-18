﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/sesv2/SESV2_EXPORTS.h>
#include <aws/sesv2/SESV2Request.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/sesv2/model/EmailContent.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/sesv2/model/Tag.h>
#include <utility>

namespace Aws
{
namespace SESV2
{
namespace Model
{

  /**
   * <p>A request to perform a predictive inbox placement test. Predictive inbox
   * placement tests can help you predict how your messages will be handled by
   * various email providers around the world. When you perform a predictive inbox
   * placement test, you provide a sample message that contains the content that you
   * plan to send to your customers. We send that message to special email addresses
   * spread across several major email providers around the world. The test takes
   * about 24 hours to complete. When the test is complete, you can use the
   * <code>GetDeliverabilityTestReport</code> operation to view the results of the
   * test.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/sesv2-2019-09-27/CreateDeliverabilityTestReportRequest">AWS
   * API Reference</a></p>
   */
  class AWS_SESV2_API CreateDeliverabilityTestReportRequest : public SESV2Request
  {
  public:
    CreateDeliverabilityTestReportRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<CreateDeliverabilityTestReportRequest> Clone() const
    {
      return Aws::MakeUnique<CreateDeliverabilityTestReportRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "CreateDeliverabilityTestReport"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>A unique name that helps you to identify the predictive inbox placement test
     * when you retrieve the results.</p>
     */
    inline const Aws::String& GetReportName() const{ return m_reportName; }

    /**
     * <p>A unique name that helps you to identify the predictive inbox placement test
     * when you retrieve the results.</p>
     */
    inline bool ReportNameHasBeenSet() const { return m_reportNameHasBeenSet; }

    /**
     * <p>A unique name that helps you to identify the predictive inbox placement test
     * when you retrieve the results.</p>
     */
    inline void SetReportName(const Aws::String& value) { m_reportNameHasBeenSet = true; m_reportName = value; }

    /**
     * <p>A unique name that helps you to identify the predictive inbox placement test
     * when you retrieve the results.</p>
     */
    inline void SetReportName(Aws::String&& value) { m_reportNameHasBeenSet = true; m_reportName = std::move(value); }

    /**
     * <p>A unique name that helps you to identify the predictive inbox placement test
     * when you retrieve the results.</p>
     */
    inline void SetReportName(const char* value) { m_reportNameHasBeenSet = true; m_reportName.assign(value); }

    /**
     * <p>A unique name that helps you to identify the predictive inbox placement test
     * when you retrieve the results.</p>
     */
    inline CreateDeliverabilityTestReportRequest& WithReportName(const Aws::String& value) { SetReportName(value); return *this;}

    /**
     * <p>A unique name that helps you to identify the predictive inbox placement test
     * when you retrieve the results.</p>
     */
    inline CreateDeliverabilityTestReportRequest& WithReportName(Aws::String&& value) { SetReportName(std::move(value)); return *this;}

    /**
     * <p>A unique name that helps you to identify the predictive inbox placement test
     * when you retrieve the results.</p>
     */
    inline CreateDeliverabilityTestReportRequest& WithReportName(const char* value) { SetReportName(value); return *this;}


    /**
     * <p>The email address that the predictive inbox placement test email was sent
     * from.</p>
     */
    inline const Aws::String& GetFromEmailAddress() const{ return m_fromEmailAddress; }

    /**
     * <p>The email address that the predictive inbox placement test email was sent
     * from.</p>
     */
    inline bool FromEmailAddressHasBeenSet() const { return m_fromEmailAddressHasBeenSet; }

    /**
     * <p>The email address that the predictive inbox placement test email was sent
     * from.</p>
     */
    inline void SetFromEmailAddress(const Aws::String& value) { m_fromEmailAddressHasBeenSet = true; m_fromEmailAddress = value; }

    /**
     * <p>The email address that the predictive inbox placement test email was sent
     * from.</p>
     */
    inline void SetFromEmailAddress(Aws::String&& value) { m_fromEmailAddressHasBeenSet = true; m_fromEmailAddress = std::move(value); }

    /**
     * <p>The email address that the predictive inbox placement test email was sent
     * from.</p>
     */
    inline void SetFromEmailAddress(const char* value) { m_fromEmailAddressHasBeenSet = true; m_fromEmailAddress.assign(value); }

    /**
     * <p>The email address that the predictive inbox placement test email was sent
     * from.</p>
     */
    inline CreateDeliverabilityTestReportRequest& WithFromEmailAddress(const Aws::String& value) { SetFromEmailAddress(value); return *this;}

    /**
     * <p>The email address that the predictive inbox placement test email was sent
     * from.</p>
     */
    inline CreateDeliverabilityTestReportRequest& WithFromEmailAddress(Aws::String&& value) { SetFromEmailAddress(std::move(value)); return *this;}

    /**
     * <p>The email address that the predictive inbox placement test email was sent
     * from.</p>
     */
    inline CreateDeliverabilityTestReportRequest& WithFromEmailAddress(const char* value) { SetFromEmailAddress(value); return *this;}


    /**
     * <p>The HTML body of the message that you sent when you performed the predictive
     * inbox placement test.</p>
     */
    inline const EmailContent& GetContent() const{ return m_content; }

    /**
     * <p>The HTML body of the message that you sent when you performed the predictive
     * inbox placement test.</p>
     */
    inline bool ContentHasBeenSet() const { return m_contentHasBeenSet; }

    /**
     * <p>The HTML body of the message that you sent when you performed the predictive
     * inbox placement test.</p>
     */
    inline void SetContent(const EmailContent& value) { m_contentHasBeenSet = true; m_content = value; }

    /**
     * <p>The HTML body of the message that you sent when you performed the predictive
     * inbox placement test.</p>
     */
    inline void SetContent(EmailContent&& value) { m_contentHasBeenSet = true; m_content = std::move(value); }

    /**
     * <p>The HTML body of the message that you sent when you performed the predictive
     * inbox placement test.</p>
     */
    inline CreateDeliverabilityTestReportRequest& WithContent(const EmailContent& value) { SetContent(value); return *this;}

    /**
     * <p>The HTML body of the message that you sent when you performed the predictive
     * inbox placement test.</p>
     */
    inline CreateDeliverabilityTestReportRequest& WithContent(EmailContent&& value) { SetContent(std::move(value)); return *this;}


    /**
     * <p>An array of objects that define the tags (keys and values) that you want to
     * associate with the predictive inbox placement test.</p>
     */
    inline const Aws::Vector<Tag>& GetTags() const{ return m_tags; }

    /**
     * <p>An array of objects that define the tags (keys and values) that you want to
     * associate with the predictive inbox placement test.</p>
     */
    inline bool TagsHasBeenSet() const { return m_tagsHasBeenSet; }

    /**
     * <p>An array of objects that define the tags (keys and values) that you want to
     * associate with the predictive inbox placement test.</p>
     */
    inline void SetTags(const Aws::Vector<Tag>& value) { m_tagsHasBeenSet = true; m_tags = value; }

    /**
     * <p>An array of objects that define the tags (keys and values) that you want to
     * associate with the predictive inbox placement test.</p>
     */
    inline void SetTags(Aws::Vector<Tag>&& value) { m_tagsHasBeenSet = true; m_tags = std::move(value); }

    /**
     * <p>An array of objects that define the tags (keys and values) that you want to
     * associate with the predictive inbox placement test.</p>
     */
    inline CreateDeliverabilityTestReportRequest& WithTags(const Aws::Vector<Tag>& value) { SetTags(value); return *this;}

    /**
     * <p>An array of objects that define the tags (keys and values) that you want to
     * associate with the predictive inbox placement test.</p>
     */
    inline CreateDeliverabilityTestReportRequest& WithTags(Aws::Vector<Tag>&& value) { SetTags(std::move(value)); return *this;}

    /**
     * <p>An array of objects that define the tags (keys and values) that you want to
     * associate with the predictive inbox placement test.</p>
     */
    inline CreateDeliverabilityTestReportRequest& AddTags(const Tag& value) { m_tagsHasBeenSet = true; m_tags.push_back(value); return *this; }

    /**
     * <p>An array of objects that define the tags (keys and values) that you want to
     * associate with the predictive inbox placement test.</p>
     */
    inline CreateDeliverabilityTestReportRequest& AddTags(Tag&& value) { m_tagsHasBeenSet = true; m_tags.push_back(std::move(value)); return *this; }

  private:

    Aws::String m_reportName;
    bool m_reportNameHasBeenSet = false;

    Aws::String m_fromEmailAddress;
    bool m_fromEmailAddressHasBeenSet = false;

    EmailContent m_content;
    bool m_contentHasBeenSet = false;

    Aws::Vector<Tag> m_tags;
    bool m_tagsHasBeenSet = false;
  };

} // namespace Model
} // namespace SESV2
} // namespace Aws
