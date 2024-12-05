import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ElementListFilterContainerComponent } from './element-list-filter-container.component';

describe('ElementListFilterContainerComponent', () => {
  let component: ElementListFilterContainerComponent;
  let fixture: ComponentFixture<ElementListFilterContainerComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ElementListFilterContainerComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ElementListFilterContainerComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
