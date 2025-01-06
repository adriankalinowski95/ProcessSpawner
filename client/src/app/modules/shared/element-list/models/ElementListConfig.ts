import {shared} from "../../shared";
import ActionType = shared.action_button.models.ActionType;
import Type = shared.icon.models.Type;
import { PageConfig } from "./pageConfig";

export interface FilterConfig {
}
export interface ContentHandlerConfig {
  buttons: shared.action_button.models.Config[]
}

export interface PaginationConfig {
    pageConfig: PageConfig;
    pageSizeOptions: number[];
}

export interface RowTableIcon {
  name: string,
  iconName: string
  actionType: ActionType,
  type: Type,
  displayName?: string,
}

export interface MultiActionButton {
  actions: RowTableIcon[]
}

export interface TableConfig {
  dbClickEdit: boolean,
  multiselect: boolean,
  sortable: boolean,
  stickyHeader: boolean
  rowTableIcons: RowTableIcon[],
  multiActionButton?: MultiActionButton
}

export interface ContentConfig {
  useContentHandler: boolean
  usePagination: boolean,
  contentHandlerConfig: ContentHandlerConfig
  paginationConfig: PaginationConfig,
  tableConfig: TableConfig
}

export interface MainConfig {
  useFilters: boolean,
  filterConfig: FilterConfig,
  contentConfig: ContentConfig
}
