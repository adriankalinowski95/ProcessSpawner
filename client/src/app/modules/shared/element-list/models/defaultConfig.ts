import {MainConfig} from "./ElementListConfig";
import {ActionType} from "../../action-button/models/action-button-config";
import {DataSource} from "./data-source";
import {shared} from "../../shared";
import {Type} from "../../icon/models/icon-config";
import DisplayedColumns = shared.element_list.models.DisplayedColumns;

export const defaultConfig: MainConfig = {
  useFilters: false,
  filterConfig: {},
  contentConfig: {
    tableConfig: {
      multiselect: true,
      rowTableIcons: [],
      multiActionButton: {
        actions: [{
            name: 'edit',
            displayName: 'Edit',
            actionType: ActionType.Edit,
            type: Type.Default,
            iconName: 'edit-outline'
        },
        {
          name: 'delete',
          displayName: 'Del',
          actionType: ActionType.Delete,
          type: Type.Error,
          iconName: 'trash-can-outline'
        },
        {
          name: 'finish_process',
          displayName: 'Finish',
          actionType: ActionType.SpecialAction1,
          type: Type.Default,
          iconName: 'start'
        }
        ]
    },
      sortable: true,
      dbClickEdit: true,
      stickyHeader: true,
    },
    usePagination: true,
    paginationConfig: {
        pageConfig: {
          length: 200,
          pageIndex: 0,
          size: 10
        },
        pageSizeOptions: [5, 10, 20, 50]
      },
    useContentHandler: true,
    contentHandlerConfig: { buttons: [{
        text: 'Create',
        disabled: false,
        type: Type.Default,
        actionType: ActionType.Create
      }, {
        text: 'Edit',
        disabled: false,
        type: Type.Info,
        actionType: ActionType.Edit
      }, {
        text: 'Reload',
        disabled: true,
        type: Type.Warn,
        actionType: ActionType.Reload
      }, {
        text: 'Delete',
        disabled: false,
        type: Type.Error,
        actionType: ActionType.Delete
      }]
    }
  }
}

export const defaultColumns: DisplayedColumns = [
  {name: 'index', displayName: 'No.'},
  {name: 'name', displayName: 'Name'},
  {name: 'weight', displayName: 'Weight'},
  {name: 'symbol', displayName: 'Symbol'}
];

export const ELEMENT_DATA: DataSource[] = [
  {index: 1, name: 'Hydrogen', weight: 1.0079, symbol: 'H'},
  {index: 2, name: 'Helium', weight: 4.0026, symbol: 'He'},
  {index: 3, name: 'Lithium', weight: 6.941, symbol: 'Li'},
  {index: 4, name: 'Beryllium', weight: 9.0122, symbol: 'Be'},
  {index: 5, name: 'Boron', weight: 10.811, symbol: 'B'},
  {index: 6, name: 'Carbon', weight: 12.0107, symbol: 'C'},
  {index: 7, name: 'Nitrogen', weight: 14.0067, symbol: 'N'},
  {index: 8, name: 'Oxygen', weight: 15.9994, symbol: 'O'},
  {index: 9, name: 'Fluorine', weight: 18.9984, symbol: 'F'},
  {index: 10, name: 'Neon', weight: 20.1797, symbol: 'Ne'},
  {index: 11, name: 'Hydrogen', weight: 1.0079, symbol: 'H'},
  {index: 12, name: 'Helium', weight: 4.0026, symbol: 'He'},
  {index: 13, name: 'Lithium', weight: 6.941, symbol: 'Li'},
  {index: 14, name: 'Beryllium', weight: 9.0122, symbol: 'Be'},
  {index: 15, name: 'Boron', weight: 10.811, symbol: 'B'},
  {index: 16, name: 'Carbon', weight: 12.0107, symbol: 'C'},
  {index: 17, name: 'Nitrogen', weight: 14.0067, symbol: 'N'},
  {index: 18, name: 'Oxygen', weight: 15.9994, symbol: 'O'},
  {index: 19, name: 'Fluorine', weight: 18.9984, symbol: 'F'},
  {index: 20, name: 'Neon', weight: 20.1797, symbol: 'Ne'},
];
