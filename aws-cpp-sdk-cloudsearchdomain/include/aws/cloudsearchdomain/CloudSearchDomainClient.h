﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/cloudsearchdomain/CloudSearchDomain_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/cloudsearchdomain/CloudSearchDomainServiceClientModel.h>
#include <aws/cloudsearchdomain/CloudSearchDomainLegacyAsyncMacros.h>

namespace Aws
{
namespace CloudSearchDomain
{
  /**
   * <p>You use the AmazonCloudSearch2013 API to upload documents to a search domain
   * and search those documents. </p> <p>The endpoints for submitting
   * <code>UploadDocuments</code>, <code>Search</code>, and <code>Suggest</code>
   * requests are domain-specific. To get the endpoints for your domain, use the
   * Amazon CloudSearch configuration service <code>DescribeDomains</code> action.
   * The domain endpoints are also displayed on the domain dashboard in the Amazon
   * CloudSearch console. You submit suggest requests to the search endpoint. </p>
   * <p>For more information, see the <a
   * href="http://docs.aws.amazon.com/cloudsearch/latest/developerguide">Amazon
   * CloudSearch Developer Guide</a>.</p>
   */
  class AWS_CLOUDSEARCHDOMAIN_API CloudSearchDomainClient : public Aws::Client::AWSJsonClient
  {
    public:
      typedef Aws::Client::AWSJsonClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudSearchDomainClient(const Aws::CloudSearchDomain::CloudSearchDomainClientConfiguration& clientConfiguration = Aws::CloudSearchDomain::CloudSearchDomainClientConfiguration(),
                                std::shared_ptr<CloudSearchDomainEndpointProviderBase> endpointProvider = Aws::MakeShared<CloudSearchDomainEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudSearchDomainClient(const Aws::Auth::AWSCredentials& credentials,
                                std::shared_ptr<CloudSearchDomainEndpointProviderBase> endpointProvider = Aws::MakeShared<CloudSearchDomainEndpointProvider>(ALLOCATION_TAG),
                                const Aws::CloudSearchDomain::CloudSearchDomainClientConfiguration& clientConfiguration = Aws::CloudSearchDomain::CloudSearchDomainClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        CloudSearchDomainClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                                std::shared_ptr<CloudSearchDomainEndpointProviderBase> endpointProvider = Aws::MakeShared<CloudSearchDomainEndpointProvider>(ALLOCATION_TAG),
                                const Aws::CloudSearchDomain::CloudSearchDomainClientConfiguration& clientConfiguration = Aws::CloudSearchDomain::CloudSearchDomainClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudSearchDomainClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CloudSearchDomainClient(const Aws::Auth::AWSCredentials& credentials,
                                const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        CloudSearchDomainClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                                const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~CloudSearchDomainClient();


        template<typename RequestT,
                 typename HandlerT,
                 typename HandlerContextT,
                 typename OperationFuncT>
        void SubmitAsync(OperationFuncT&& operationFunc,
                         const RequestT& request,
                         const HandlerT& handler,
                         const HandlerContextT& context)
        {
            Aws::Client::MakeAsyncOperation(std::forward<OperationFuncT>(operationFunc), this, request, handler, context, m_executor.get());
        }

        template<typename RequestT,
                 typename HandlerT,
                 typename HandlerContextT,
                 typename OperationFuncT>
        void SubmitAsync(OperationFuncT&& operationFunc,
                         RequestT& request,
                         const HandlerT& handler,
                         const HandlerContextT& context)
        {
            Aws::Client::MakeAsyncStreamingOperation(std::forward<OperationFuncT>(operationFunc), this, request, handler, context, m_executor.get());
        }

        template<typename RequestT,
                 typename OperationFuncT>
        auto SubmitCallable(OperationFuncT&& operationFunc,
                            const RequestT& request) -> std::future<decltype((this->*operationFunc)(request))>
        {
            return Aws::Client::MakeCallableOperation(ALLOCATION_TAG, operationFunc, this, request, m_executor.get());
        }

        template<typename RequestT,
                 typename OperationFuncT>
        auto SubmitCallable(OperationFuncT&& operationFunc,
                            RequestT& request) -> std::future<decltype((this->*operationFunc)(request))>
        {
            return Aws::Client::MakeCallableStreamingOperation(ALLOCATION_TAG, operationFunc, this, request, m_executor.get());
        }


        /**
         * <p>Retrieves a list of documents that match the specified search criteria. How
         * you specify the search criteria depends on which query parser you use. Amazon
         * CloudSearch supports four query parsers:</p> <ul> <li><code>simple</code>:
         * search all <code>text</code> and <code>text-array</code> fields for the
         * specified string. Search for phrases, individual terms, and prefixes. </li>
         * <li><code>structured</code>: search specific fields, construct compound queries
         * using Boolean operators, and use advanced features such as term boosting and
         * proximity searching.</li> <li><code>lucene</code>: specify search criteria using
         * the Apache Lucene query parser syntax.</li> <li><code>dismax</code>: specify
         * search criteria using the simplified subset of the Apache Lucene query parser
         * syntax defined by the DisMax query parser.</li> </ul> <p>For more information,
         * see <a
         * href="http://docs.aws.amazon.com/cloudsearch/latest/developerguide/searching.html">Searching
         * Your Data</a> in the <i>Amazon CloudSearch Developer Guide</i>.</p> <p>The
         * endpoint for submitting <code>Search</code> requests is domain-specific. You
         * submit search requests to a domain's search endpoint. To get the search endpoint
         * for your domain, use the Amazon CloudSearch configuration service
         * <code>DescribeDomains</code> action. A domain's endpoints are also displayed on
         * the domain dashboard in the Amazon CloudSearch console. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/cloudsearchdomain-2013-01-01/Search">AWS
         * API Reference</a></p>
         */
        virtual Model::SearchOutcome Search(const Model::SearchRequest& request) const;


        /**
         * <p>Retrieves autocomplete suggestions for a partial query string. You can use
         * suggestions enable you to display likely matches before users finish typing. In
         * Amazon CloudSearch, suggestions are based on the contents of a particular text
         * field. When you request suggestions, Amazon CloudSearch finds all of the
         * documents whose values in the suggester field start with the specified query
         * string. The beginning of the field must match the query string to be considered
         * a match. </p> <p>For more information about configuring suggesters and
         * retrieving suggestions, see <a
         * href="http://docs.aws.amazon.com/cloudsearch/latest/developerguide/getting-suggestions.html">Getting
         * Suggestions</a> in the <i>Amazon CloudSearch Developer Guide</i>. </p> <p>The
         * endpoint for submitting <code>Suggest</code> requests is domain-specific. You
         * submit suggest requests to a domain's search endpoint. To get the search
         * endpoint for your domain, use the Amazon CloudSearch configuration service
         * <code>DescribeDomains</code> action. A domain's endpoints are also displayed on
         * the domain dashboard in the Amazon CloudSearch console. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/cloudsearchdomain-2013-01-01/Suggest">AWS
         * API Reference</a></p>
         */
        virtual Model::SuggestOutcome Suggest(const Model::SuggestRequest& request) const;


        /**
         * <p>Posts a batch of documents to a search domain for indexing. A document batch
         * is a collection of add and delete operations that represent the documents you
         * want to add, update, or delete from your domain. Batches can be described in
         * either JSON or XML. Each item that you want Amazon CloudSearch to return as a
         * search result (such as a product) is represented as a document. Every document
         * has a unique ID and one or more fields that contain the data that you want to
         * search and return in results. Individual documents cannot contain more than 1 MB
         * of data. The entire batch cannot exceed 5 MB. To get the best possible upload
         * performance, group add and delete operations in batches that are close the 5 MB
         * limit. Submitting a large volume of single-document batches can overload a
         * domain's document service. </p> <p>The endpoint for submitting
         * <code>UploadDocuments</code> requests is domain-specific. To get the document
         * endpoint for your domain, use the Amazon CloudSearch configuration service
         * <code>DescribeDomains</code> action. A domain's endpoints are also displayed on
         * the domain dashboard in the Amazon CloudSearch console. </p> <p>For more
         * information about formatting your data for Amazon CloudSearch, see <a
         * href="http://docs.aws.amazon.com/cloudsearch/latest/developerguide/preparing-data.html">Preparing
         * Your Data</a> in the <i>Amazon CloudSearch Developer Guide</i>. For more
         * information about uploading data for indexing, see <a
         * href="http://docs.aws.amazon.com/cloudsearch/latest/developerguide/uploading-data.html">Uploading
         * Data</a> in the <i>Amazon CloudSearch Developer Guide</i>. </p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/cloudsearchdomain-2013-01-01/UploadDocuments">AWS
         * API Reference</a></p>
         */
        virtual Model::UploadDocumentsOutcome UploadDocuments(const Model::UploadDocumentsRequest& request) const;



      void OverrideEndpoint(const Aws::String& endpoint);
      std::shared_ptr<CloudSearchDomainEndpointProviderBase>& accessEndpointProvider();
    private:
      void init(const CloudSearchDomainClientConfiguration& clientConfiguration);

      CloudSearchDomainClientConfiguration m_clientConfiguration;
      std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
      std::shared_ptr<CloudSearchDomainEndpointProviderBase> m_endpointProvider;
  };

} // namespace CloudSearchDomain
} // namespace Aws
