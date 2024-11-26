import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import {
  ElementListContentHandlerComponent
} from "./element-list-content-container/element-list-content-handler/element-list-content-handler.component";
import {
  ElementListFilterContainerComponent
} from "./element-list-filter-container/element-list-filter-container.component";
import {
  ElementListContentComponent
} from "./element-list-content-container/element-list-content/element-list-content.component";
import {
  ElementListContentPaginatorComponent
} from "./element-list-content-container/element-list-content-paginator/element-list-content-paginator.component";
import {
  ElementListContentContainerComponent
} from "./element-list-content-container/element-list-content-container.component";
import {ElementListComponent} from "./element-list.component";
import {ActionButtonModule} from "../action-button/action-button.module";
import {MatTableModule} from '@angular/material/table';
import {MatCheckboxModule} from '@angular/material/checkbox';
import {IconModule} from "../icon/icon.module";
import {MatSortModule} from "@angular/material/sort";
import { MatMenuModule } from '@angular/material/menu';
import { MatButtonModule } from '@angular/material/button';
import { MatTooltipModule } from '@angular/material/tooltip';
import { MatIcon, MatIconModule } from '@angular/material/icon';
import { MatPaginatorModule } from '@angular/material/paginator';


@NgModule({
  declarations: [
    ElementListContentHandlerComponent,
    ElementListFilterContainerComponent,
    ElementListContentComponent,
    ElementListContentPaginatorComponent,
    ElementListContentContainerComponent,
    ElementListComponent
  ],
  imports: [
    CommonModule,
    ActionButtonModule,
    MatTableModule,
    MatCheckboxModule,
    MatSortModule,
    MatMenuModule,
    MatButtonModule,
    MatTooltipModule,
    MatIconModule,
    MatPaginatorModule,
    IconModule
  ],
  exports: [
    ElementListComponent
  ]
})
export class ElementListModule { }
