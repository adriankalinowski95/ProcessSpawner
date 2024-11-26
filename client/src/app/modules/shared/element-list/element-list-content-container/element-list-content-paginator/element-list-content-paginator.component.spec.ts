import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ElementListContentPaginatorComponent } from './element-list-content-paginator.component';

describe('ElementListContentPaginatorComponent', () => {
  let component: ElementListContentPaginatorComponent;
  let fixture: ComponentFixture<ElementListContentPaginatorComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ElementListContentPaginatorComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ElementListContentPaginatorComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
