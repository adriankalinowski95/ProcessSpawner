import * as BaseResponse from "./response/base-response"
import * as BasePaginationResponse from "./response/base-pagination-response"
import * as BaseFieldValidationResponse from "./response/base-field-validation-response"
import * as ObjectOperationResponse from "./response/object-operation-response"
import * as ObjectResponse from "./response/object-response"
import * as BasePaginationRequest from "./request/base-pagination-request"
import * as FormErrorHandlingService from "./utils/form-error-handling"
import * as Base64Conv from "./utils/base64-converter"
import * as IsNotNullOrUnd from "./utils/validators"
import * as CommonMeth from "./utils/common-methods"
import * as CommonDefinitions from "./definitions"
import * as _BaseCrudService from "./request/base-crud-service"
import * as _ExtendCrudService from "./request/extended-crud-service"
import * as _ObjectsResponse from "./response/objects-response"
import * as _NotificationService from "./services/notification.service"
import * as _BaseStateHolder from "./services/base-state-holder"
import * as _ExtendedStateHolder from "./services/extend-state-holder"

import * as ElementListModuleImport from "./element-list/element-list.module"
import * as ElementListConfigModel from "./element-list/models/ElementListConfig";
import * as ActionButtonConfig from "./action-button/models/action-button-config";
import * as DataSourceModel from "./element-list/models/data-source";
import * as DisplayedColumnsModel from "./element-list/models/displayed-columns";
import * as IconConfig from "./icon/models/icon-config";


export namespace shared {
    export namespace response {
        export import Base = BaseResponse.BaseResponse;
        export import BasePagination = BasePaginationResponse.BasePaginationResponse;
        export import BaseFieldValidation = BaseFieldValidationResponse.BaseFieldValidationResponse;
        export import Object = ObjectResponse.ObjectResponse;
        export import Objects = _ObjectsResponse.ObjectsResponse;
        export import ObjectOperation = ObjectOperationResponse.ObjectOperationResponse;
    }

    export namespace request {
        export import BasePagination = BasePaginationRequest.BasePaginationRequest;
    }

    export namespace enums {
        export import BaseResponseStatus = BaseResponse.BaseResponseStatus;
    }

    export namespace services {
        export import FormErrorHandling = FormErrorHandlingService.FormErrorHandlingService;
        export import BaseCrudService = _BaseCrudService.BaseCrudService;
        export import ExtendedCrudService = _ExtendCrudService.ExtendedCrudService;
        export import NotificationService = _NotificationService.NotificationService;
        export import BaseStateHolder = _BaseStateHolder.BaseStateHolder;
        export import ExtendStateHolder = _ExtendedStateHolder.ExtendStateHolder;
    }

    export import definitions = CommonDefinitions.definitions
    export import isNotNullOrUndefined = IsNotNullOrUnd.isNotNullOrUndefined;
    export import Base64Converter = Base64Conv.Base64Converter;
    export import getRandomString = CommonMeth.getRandomString;
    export import getRandomNumber = CommonMeth.getRandomNumber;

    export namespace element_list {
        export namespace models {
          export import MainConfig = ElementListConfigModel.MainConfig;
          export import FilterConfig = ElementListConfigModel.FilterConfig;
          export import PaginationConfig = ElementListConfigModel.PaginationConfig;
          export import ContentHandlerConfig = ElementListConfigModel.ContentHandlerConfig;
          export import ContentConfig = ElementListConfigModel.ContentConfig;
          export import RowTableIcon = ElementListConfigModel.RowTableIcon;
          export import DataSource = DataSourceModel.DataSource;
          export import DisplayedColumns = DisplayedColumnsModel.DisplayedColumns;
        }
      }
      export namespace action_button {
        export namespace models {
          export import Config = ActionButtonConfig.Config
          export import Type = ActionButtonConfig.Type
          export import ActionType = ActionButtonConfig.ActionType
        }
      }
      export namespace icon {
        export namespace models {
          export import Type = IconConfig.Type
        }
      }
}